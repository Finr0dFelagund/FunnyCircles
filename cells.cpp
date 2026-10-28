#include "cells.h"

#include <QCoreApplication>
#include <QEventLoop>

#include "globalvars.h"

Cells::Cells()
    :tickDone(true), stopRequested(false), threadsDoneWork(0), pendingWork(0), tickRunning(false)
{
    /*for(int i = 0; i < GLOBALVARS::numOfThreads; i++)
    {
        threadWorkers.append(new CellTickProcesser(&cells));
        threads.append(new QThread());
        threadWorkers[i]->moveToThread(threads[i]);
        connect(threadWorkers[i], SIGNAL(calculationReady(QThread*)), this, SLOT(calculationDoneSlot(QThread*)));
        connect(threadWorkers[i], SIGNAL(applyingDeltasReady(QThread*)), this, SLOT(applyDeltasDoneSlot(QThread*)));
        connect(this, SIGNAL(calculate(QThread*,quint16,quint16)), threadWorkers[i], SLOT(calculate(QThread*,quint16,quint16)));
        connect(this, SIGNAL(applyDeltas(QThread*,quint16,quint16)), threadWorkers[i], SLOT(applyDeltas(QThread*,quint16,quint16)));
        threads[i]->start(QThread::LowPriority);
    }*/
}

Cells::~Cells()
{
    shutdown_threads();
}

void Cells::shutdown_threads()
{
    while(!threads.isEmpty())
    {
        QThread* thread = threads.takeLast();
        CellTickProcesser* worker = threadWorkers.takeLast();
        thread->quit();
        thread->wait();
        delete worker;
        delete thread;
    }
}

bool Cells::is_busy() const
{
    return !tickDone;
}

void Cells::stop()
{
    stopRequested = true;
    // Ожидание завершения текущего такта: сигналы потоков доставляются в очередь
    // событий главного потока, поэтому она обрабатывается до закрытия такта.
    while(tickRunning.load())
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
    }
    tickDone = true;
    threadsDoneWork = 0;
    pendingWork = 0;
    stopRequested = false;
}

void Cells::adjustThreads()
{
    GLOBALVARS::numOfThreads = (cells.size() + (GLOBALVARS::cellsPerThread+1)/2)/GLOBALVARS::cellsPerThread;
    GLOBALVARS::numOfThreads = GLOBALVARS::numOfThreads == 0 ? 1 : GLOBALVARS::numOfThreads;
    while(threads.size() < GLOBALVARS::numOfThreads)
    {
        threadWorkers.append(new CellTickProcesser(&cells));
        threads.append(new QThread());
        threadWorkers.last()->moveToThread(threads.last());
        connect(threadWorkers.last(), SIGNAL(calculationReady(QThread*)), this, SLOT(calculationDoneSlot(QThread*)));
        connect(threadWorkers.last(), SIGNAL(applyingDeltasReady(QThread*)), this, SLOT(applyDeltasDoneSlot(QThread*)));
        connect(this, SIGNAL(calculate(QThread*,quint16,quint16)), threadWorkers.last(), SLOT(calculate(QThread*,quint16,quint16)));
        connect(this, SIGNAL(applyDeltas(QThread*,quint16,quint16)), threadWorkers.last(), SLOT(applyDeltas(QThread*,quint16,quint16)));
        threads.last()->start(QThread::LowPriority);
    }
    while(threads.size() > GLOBALVARS::numOfThreads)
    {
        // Поток завершается до уничтожения обработчика, иначе объект уничтожается
        // во время выполнения слота в чужом потоке.
        QThread* thread = threads.takeLast();
        CellTickProcesser* worker = threadWorkers.takeLast();
        disconnect(this, nullptr, worker, nullptr);
        thread->quit();
        thread->wait();
        delete worker;
        delete thread;
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
        for(quint16 k = 0; k < cells[b]->connectionsInd.size(); k++)
        {
            if(cells[b]->connectionsInd[k] == a)
            {
                result = false;
                break;
            }
        }
        if(!result)
        {
            return result;
        }
        cells[a <= b ? a : b]->mutex.lock();
        cells[a <= b ? b : a]->mutex.lock();
        {
            const bool created = (consumer->numberOfTypeConnections[dealer->type->number] < consumer->type->maxConnectionsNumber[dealer->type->number] || consumer->type->maxConnectionsNumber[dealer->type->number] < 0) &&
                (dealer->numberOfTypeConnections[consumer->type->number] < dealer->type->maxConnectionsNumber[consumer->type->number] || dealer->type->maxConnectionsNumber[consumer->type->number] < 0) &&
                (consumer->numberOfTypeConnections.last() < consumer->type->maxConnectionsNumber.last() || consumer->type->maxConnectionsNumber.last() < 0) &&
                (dealer->numberOfTypeConnections.last() < dealer->type->maxConnectionsNumber.last() || dealer->type->maxConnectionsNumber.last() < 0);
            if(created)
            {
                consumer->connectionsInd.append(b);
                consumer->numberOfTypeConnections[dealer->type->number]++;
                consumer->numberOfTypeConnections.last()++;
                dealer->connectionsInd.append(a);
                dealer->numberOfTypeConnections[consumer->type->number]++;
                dealer->numberOfTypeConnections.last()++;
            }
            result = created;
        }
        cells[a <= b ? b : a]->mutex.unlock();
        cells[a <= b ? a : b]->mutex.unlock();
    }
    else
    {
        result = false;
    }
    return result;
}

void Cells::applyDeltasDoneSlot(QThread* thr)
{
    Q_UNUSED(thr);
    // threadsDoneWork считает отчитавшиеся потоки текущей фазы, pendingWork — сколько
    // их ожидается. Счётчик обнуляется при переходе к следующей фазе такта.
    threadsDoneWork++;
    if(threadsDoneWork < pendingWork)
    {
        return;
    }
    threadsDoneWork = 0;
    pendingWork = 0;
    tickRunning.store(false);
    tickDone = true;
}

void Cells::calculationDoneSlot(QThread* thr)
{
    Q_UNUSED(thr);
    threadsDoneWork++;
    if(threadsDoneWork < pendingWork)
    {
        return;
    }
    threadsDoneWork = 0;
    if(stopRequested || threads.isEmpty())
    {
        // Прерванный такт не применяет дельты: данные остаются в состоянии
        // последнего завершённого такта.
        pendingWork = 0;
        tickRunning.store(false);
        tickDone = true;
        return;
    }
    const int applied_threads = static_cast<int>(threads.size());
    pendingWork = static_cast<quint8>(applied_threads);
    for(int i = 0; i < applied_threads - 1; i++)
    {
        emit applyDeltas(threads[i], i*(cells.size()/applied_threads), (i+1)*(cells.size()/applied_threads));
    }
    emit applyDeltas(threads[applied_threads - 1], (applied_threads - 1)*(cells.size()/applied_threads), cells.size());
}

void Cells::tick()
{
    if(tickDone && !stopRequested && !threads.isEmpty())
    {
        tickDone = false;
        tickRunning.store(true);
        threadsDoneWork = 0;
        pendingWork = static_cast<quint8>(threads.size());
        for(int i = 0; i < threads.size() - 1; i++)
        {
            emit calculate(threads[i], i*(cells.size()/GLOBALVARS::numOfThreads), (i+1)*(cells.size()/GLOBALVARS::numOfThreads));
        }
        emit calculate(threads.last(), (threads.size() - 1)*(cells.size()/GLOBALVARS::numOfThreads), cells.size());
    }
}
