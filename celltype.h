#ifndef CELLTYPE_H
#define CELLTYPE_H

#include <QBrush>
#include <QtCore>

class CellType
{
public:
    CellType(QBrush brushx, qreal (*forcex)(qreal, qreal, qreal), QVector<qint8> interDirectx, qreal sizex, qreal massx, qreal frictionCoeffx, qreal hardnessFactorx, QVector<qint8> maxConnectionsNumberx);

    QBrush brush;
    qreal size;
    qreal mass;
    qreal frictionCoeff;
    qreal hardnessFactor;

    qreal (*force)(qreal x, qreal m1, qreal m2);
    QVector<qint8> interactDirection;//from this to another 1 - push away, -1 = attract
    QVector<qint8> maxConnectionsNumber;
    quint8 number;
    QString name;

    static QVector<CellType*> types;

    static CellType* newType(QBrush brushx, qreal (*forcex)(qreal, qreal, qreal), QVector<qint8> interDirectx, qreal sizex, qreal massx, qreal frictionCoeffx, qreal hardnessFactorx, QVector<qint8> maxConnectionsNumberx,
                             QVector<qint8> interDirectOtherx, QVector<qint8> maxConnectionsNumberOtherx, QString namex);
    static void removeType(CellType *id);
};

#endif // CELLTYPE_H
