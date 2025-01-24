#ifndef CELLS_H
#define CELLS_H

#include <QVector>
#include <QVector2D>
#include "celltickprocesser.h"


class Cells : public QObject
{
    Q_OBJECT
public:
    Cells();
    QVector<Cell*> cells;
    QVector<CellTickProcesser*> threadWorkers;
    QVector<QThread*> threads;
    bool tickDone;
    quint8 threadsDoneWork;

    void addCell(QVector2D pos, QVector2D spd, CellType* tp);
    void deleteCell(quint16 id);
    void removeConnect(quint16 ind);
    void removeAllConnections(quint16 ind);//--

    bool tryToConnect(quint16 a, quint16 b);//--
    void removeConnect(quint16 a, quint16 b);//--

    void tick();

public slots:
    void calculationDoneSlot(QThread *thr);
    void applyDeltasDoneSlot(QThread *thr);
signals:
    void calculate(QThread* thr, quint16 startInd, quint16 endInd);
    void applyDeltas(QThread* thr, quint16 startInd, quint16 endInd);
};

#endif // CELLS_H
