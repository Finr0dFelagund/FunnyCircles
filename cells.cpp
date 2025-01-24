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
    /*for(quint16 i = 0; i < connectionsInd[0].size(); i++)
    {
        if(connectionsInd[0][i] > id)
        {
            connectionsInd[0][i]--;
        }
        if(connectionsInd[1][i] > id)
        {
            connectionsInd[1][i]--;
        }
    }*/
}

void Cells::removeConnect(quint16 ind)//ind - index of bound
{
    /*cells[connectionsInd[0][ind]]->numberOfTypeConnections[cells[connectionsInd[1][ind]]->type->number]--;
    cells[connectionsInd[0][ind]]->numberOfTypeConnections.last()--;
    cells[connectionsInd[1][ind]]->numberOfTypeConnections[cells[connectionsInd[0][ind]]->type->number]--;
    cells[connectionsInd[1][ind]]->numberOfTypeConnections.last()--;
    connectionsInd[0].remove(ind);
    connectionsInd[1].remove(ind);*/;
}

void Cells::removeAllConnections(quint16 ind)//ind - ind of cell
{
    /*for(quint16 i = 0; i < connectionsInd[0].size(); i++)
    {
        if(connectionsInd[0][i] == ind || connectionsInd[1][i] == ind)
        {
            removeConnect(i);
            i--;
        }
    }*/;
}

void Cells::removeConnect(quint16 a, quint16 b)
{
    /*for(quint16 i = 0; i < connectionsInd[0].size(); i++)
    {
        if((connectionsInd[0][i] == b && connectionsInd[1][i] == a) || (connectionsInd[0][i] == a && connectionsInd[1][i] == b))
        {
            removeConnect(i);
            break;
        }
    }*/;
}

bool Cells::tryToConnect(quint16 a, quint16 b)
{
    bool result = false;
    /*if(GLOBALVARS::enableBounds)
    {
        Cell *consumer = cells[a], *dealer = cells[b];
        qreal distance = consumer->position.distanceToPoint(dealer->position) - (consumer->type->size + dealer->type->size)/2;
        qreal boundEnergy = GLOBALVARS::boundStiffnessFactor * ((distance > 0) ? distance : 0) / sqrt(sqrt(consumer->type->mass + dealer->type->mass));
        if(a!=b && boundEnergy < GLOBALVARS::maxBoundEnergy)
        {
            for(quint16 i = 0; i < connectionsInd[0].size(); i++)
            {
                if((connectionsInd[0][i] == a && connectionsInd[1][i] == b) || (connectionsInd[0][i] == b && connectionsInd[1][i] == a))
                    return false;
            }
            if((consumer->numberOfTypeConnections[dealer->type->number] < consumer->type->maxConnectionsNumber[dealer->type->number] || consumer->type->maxConnectionsNumber[dealer->type->number] < 0) &&
                (dealer->numberOfTypeConnections[consumer->type->number] < dealer->type->maxConnectionsNumber[consumer->type->number] || dealer->type->maxConnectionsNumber[consumer->type->number] < 0) &&
                (consumer->numberOfTypeConnections.last() < consumer->type->maxConnectionsNumber.last() || consumer->type->maxConnectionsNumber.last() < 0) &&
                (dealer->numberOfTypeConnections.last() < dealer->type->maxConnectionsNumber.last() || dealer->type->maxConnectionsNumber.last() < 0))
            {
                connectionsInd[0].append(a);
                connectionsInd[1].append(b);
                consumer->numberOfTypeConnections[dealer->type->number]++;
                consumer->numberOfTypeConnections.last()++;
                dealer->numberOfTypeConnections[consumer->type->number]++;
                dealer->numberOfTypeConnections.last()++;
                result = true;
            }
        }
    }*/
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
