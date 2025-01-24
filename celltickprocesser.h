#ifndef CELLTICKPROCESSER_H
#define CELLTICKPROCESSER_H

#include <QObject>
#include <QThread>
#include <QVector2D>
#include "celltype.h"

struct Cell
{
    QVector2D acceleration;
    QVector2D speed;
    QVector2D position;
    QVector2D sumForce;
    CellType* type;
    QVector<quint8> numberOfTypeConnections;//last is the whole connections
    bool moveAble;
    bool collisionAble;
    QVector<quint16> connectionsInd;
    QMutex mutex;
};

class CellTickProcesser : public QObject
{
    Q_OBJECT
public:
    CellTickProcesser(QVector<Cell*> *cellArray_);
    QVector<Cell*> *cellArray;

    void calcForces(quint16 startInd, quint16 endInd);
    void createConnections(quint16 startInd, quint16 endInd);
    void removeConnections(quint16 startInd, quint16 endInd);

public slots:
    void calculate(QThread* thr, quint16 startInd, quint16 endInd);
    void applyDeltas(QThread* thr, quint16 startInd, quint16 endInd);
signals:
    void calculationReady(QThread *thr);
    void applyingDeltasReady(QThread *thr);
};

#endif // CELLTICKPROCESSER_H
