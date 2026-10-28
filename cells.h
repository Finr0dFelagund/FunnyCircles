#ifndef CELLS_H
#define CELLS_H

#include <atomic>

#include <QObject>
#include <QThread>
#include <QVector>
#include <QVector2D>

#include "celltickprocesser.h"
#include "celltype.h"

class Cells : public QObject
{
    Q_OBJECT
public:
    Cells();
    ~Cells();
    QVector<Cell*> cells;
    QVector<CellTickProcesser*> threadWorkers;
    QVector<QThread*> threads;
    bool tickDone;
    bool stopRequested;
    quint8 threadsDoneWork;
    quint8 pendingWork;
    std::atomic<bool> tickRunning;

    void addCell(QVector2D pos, QVector2D spd, CellType* tp);
    void deleteCell(quint16 id);
    void removeAllConnections(quint16 ind);
    bool tryToConnect(quint16 a, quint16 b);
    void removeConnect(quint16 a, quint16 b);
    void adjustThreads();
    void shutdown_threads();

    void tick();
    void stop();
    bool is_busy() const;

public slots:
    void calculationDoneSlot(QThread *thr);
    void applyDeltasDoneSlot(QThread *thr);
signals:
    void calculate(QThread* thr, quint16 startInd, quint16 endInd);
    void applyDeltas(QThread* thr, quint16 startInd, quint16 endInd);
};

#endif // CELLS_H
