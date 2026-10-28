#ifndef CORE_CELL_H
#define CORE_CELL_H

#include <QMutex>
#include <QVector>
#include <QVector2D>

#include "celltype.h"

// Пакет данных одной клетки: кинематика, тип, связи и флаги поведения.
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

#endif // CORE_CELL_H
