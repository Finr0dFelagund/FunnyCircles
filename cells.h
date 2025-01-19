#ifndef CELLS_H
#define CELLS_H

#include <QVector>
#include <QVector2D>
#include "celltype.h"
#include "globalvars.h"

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
};

class Cells
{
public:
    Cells();
    QVector<Cell*> cells;
    QVector<QVector<quint16>> connectionsInd; //2*connections number with ell indexes

    void addCell(QVector2D pos, QVector2D spd, CellType* tp);
    void deleteCell(quint16 id);
    void removeConnect(quint16 ind);
    //void removeAllConnections();

    bool tryToConnect(quint16 a, quint16 b);
    void createConnections();
    void calcForces();
    void applyDeltas();

    void onTick(quint8 numOfTicks);
};

#endif // CELLS_H
