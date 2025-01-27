#include "cells.h"
#include "globalvars.h"

Cells::Cells()
    :tickDone(true), threadsDoneWork(0)
{
    for(int i = 0; i < GLOBALVARS::numOfThreads; i++)
    {
        threadWorkers.append(new CellTickProcesser(&cells));
        threads.append(new QThread());
        threadWorkers[i]->moveToThread(threads[i]);
        connect(threadWorkers[i], SIGNAL(calculationReady(QThread*)), this, SLOT(calculationDoneSlot(QThread*)));
        connect(threadWorkers[i], SIGNAL(applyingDeltasReady(QThread*)), this, SLOT(applyDeltasDoneSlot(QThread*)));
        connect(this, SIGNAL(calculate(QThread*,quint16,quint16)), threadWorkers[i], SLOT(calculate(QThread*,quint16,quint16)));
        connect(this, SIGNAL(applyDeltas(QThread*,quint16,quint16)), threadWorkers[i], SLOT(applyDeltas(QThread*,quint16,quint16)));
        threads[i]->start(QThread::LowPriority);
    }
}

void Cells::addCell(QVector2D pos, QVector2D spd, CellType* tp)
{
    cells.append(new Cell{QVector2D(0, 0), spd, pos, QVector2D(0, 0), tp, QVector<quint8>(tp->maxConnectionsNumber.size()), true, true});
}

void Cells::deleteCell(quint16 id)
{
    removeAllConnections(id);
    delete cells[id];
    cells.remove(id);
    for(quint16 i = 0; i < cells.size(); i++)
    {
        for(quint8 j = 0; j < cells[i]->connectionsInd.size(); j++)
        {
            if(cells[i]->connectionsInd[j] > id)
            {
                cells[i]->connectionsInd[j]--;
            }
        }
    }
}

void Cells::removeAllConnections(quint16 ind)//ind - ind of cell
{
    quint16 ind2;
    while(cells[ind]->connectionsInd.size() > 0)
    {
        ind2 = cells[ind]->connectionsInd[0];
        cells[ind <= ind2 ? ind : ind2]->mutex.lock();
        cells[ind <= ind2 ? ind2 : ind]->mutex.lock();
        cells[ind]->connectionsInd.remove(0);
        cells[ind2]->connectionsInd.remove(cells[ind2]->connectionsInd.indexOf(ind));
        cells[ind]->numberOfTypeConnections[cells[ind2]->type->number]--;
        cells[ind]->numberOfTypeConnections.last()--;
        cells[ind2]->numberOfTypeConnections[cells[ind]->type->number]--;
        cells[ind2]->numberOfTypeConnections.last()--;
        cells[ind <= ind2 ? ind2 : ind]->mutex.unlock();
        cells[ind <= ind2 ? ind : ind2]->mutex.unlock();
    }
}

void Cells::removeConnect(quint16 a, quint16 b)
{
    for(quint8 i = 0; i < cells[a]->connectionsInd.size(); i++)
    {
        cells[a <= b ? a : b]->mutex.lock();
        cells[a <= b ? b : a]->mutex.lock();
        if(cells[a]->connectionsInd[i] == b)
        {
            cells[a]->connectionsInd.remove(i);
            cells[b]->connectionsInd.remove(cells[b]->connectionsInd.indexOf(a));
            cells[a]->numberOfTypeConnections[cells[b]->type->number]--;
            cells[a]->numberOfTypeConnections.last()--;
            cells[b]->numberOfTypeConnections[cells[a]->type->number]--;
            cells[b]->numberOfTypeConnections.last()--;
            cells[a <= b ? b : a]->mutex.unlock();
            cells[a <= b ? a : b]->mutex.unlock();
            break;
        }
        cells[a <= b ? b : a]->mutex.unlock();
        cells[a <= b ? a : b]->mutex.unlock();
    }
}

bool Cells::tryToConnect(quint16 a, quint16 b)
{
    bool result = true;
    Cell* consumer = cells[a];
    Cell* dealer = cells[b];
    qreal distance = consumer->position.distanceToPoint(dealer->position) - (consumer->type->size + dealer->type->size)/2;
    qreal boundEnergy = GLOBALVARS::boundStiffnessFactor * ((distance > 0) ? distance : 0) / sqrt(sqrt(consumer->type->mass + dealer->type->mass));
    if(consumer != dealer && boundEnergy < GLOBALVARS::maxBoundEnergy)
    {
        for(quint16 k = 0; k < consumer->connectionsInd.size(); k++)
        {
            if(consumer->connectionsInd[k] == b)
            {
                result = false;
                break;
            }
        }
        if(!result)
        {
            return result;
        }
        else
        {
            cells[a <= b ? a : b]->mutex.lock();
            cells[a <= b ? b : a]->mutex.lock();
            if((consumer->numberOfTypeConnections[dealer->type->number] < consumer->type->maxConnectionsNumber[dealer->type->number] || consumer->type->maxConnectionsNumber[dealer->type->number] < 0) &&
                (dealer->numberOfTypeConnections[consumer->type->number] < dealer->type->maxConnectionsNumber[consumer->type->number] || dealer->type->maxConnectionsNumber[consumer->type->number] < 0) &&
                (consumer->numberOfTypeConnections.last() < consumer->type->maxConnectionsNumber.last() || consumer->type->maxConnectionsNumber.last() < 0) &&
                (dealer->numberOfTypeConnections.last() < dealer->type->maxConnectionsNumber.last() || dealer->type->maxConnectionsNumber.last() < 0))
            {
                consumer->connectionsInd.append(b);
                consumer->numberOfTypeConnections[dealer->type->number]++;
                consumer->numberOfTypeConnections.last()++;
                dealer->connectionsInd.append(a);
                dealer->numberOfTypeConnections[consumer->type->number]++;
                dealer->numberOfTypeConnections.last()++;
            }
            cells[a <= b ? b : a]->mutex.unlock();
            cells[a <= b ? a : b]->mutex.unlock();
        }
    }
    return result;
}

void Cells::applyDeltasDoneSlot(QThread* thr)
{
    threadsDoneWork++;
    if(threadsDoneWork < GLOBALVARS::numOfThreads)
    {
        return;
    }
    threadsDoneWork = 0;
    tickDone = true;
}

void Cells::calculationDoneSlot(QThread* thr)
{
    threadsDoneWork++;
    if(threadsDoneWork < GLOBALVARS::numOfThreads)
    {
        return;
    }
    for(int i = 0; i < threads.size() - 1; i++)
    {
        emit applyDeltas(threads[i], i*(cells.size()/GLOBALVARS::numOfThreads), (i+1)*(cells.size()/GLOBALVARS::numOfThreads));
    }
    emit applyDeltas(threads.last(), (threads.size() - 1)*(cells.size()/GLOBALVARS::numOfThreads), cells.size());
    threadsDoneWork = 0;
}

void Cells::tick()
{
    if(tickDone)
    {
        tickDone = false;
        for(int i = 0; i < threads.size() - 1; i++)
        {
            emit calculate(threads[i], i*(cells.size()/GLOBALVARS::numOfThreads), (i+1)*(cells.size()/GLOBALVARS::numOfThreads));
        }
        emit calculate(threads.last(), (threads.size() - 1)*(cells.size()/GLOBALVARS::numOfThreads), cells.size());
    }
}
