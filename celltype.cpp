#include "celltype.h"

QVector<CellType*> CellType::types;

CellType::CellType(QBrush brushx, qreal (*forcex)(qreal, qreal, qreal), QVector<qint8> interDirectx, qreal sizex, qreal massx, qreal frictionCoeffx, qreal hardnessFactorx, QVector<qint8> maxConnectionsNumberx)
    :brush(brushx), size(sizex), mass(massx), frictionCoeff(frictionCoeffx), hardnessFactor(hardnessFactorx), force(forcex), interactDirection(interDirectx), maxConnectionsNumber(maxConnectionsNumberx)
{
    number = types.size();
    types.append(this);
}

CellType* CellType::newType(QBrush brushx, qreal (*forcex)(qreal, qreal, qreal), QVector<qint8> interDirectx, qreal sizex, qreal massx, qreal frictionCoeffx, qreal hardnessFactorx,
                            QVector<qint8> maxConnectionsNumberx, QVector<qint8> interDirectOtherx, QVector<qint8> maxConnectionsNumberOtherx, QString namex)
{
    CellType *res = nullptr;
    if(interDirectx.size() == types.size()+1 && maxConnectionsNumberx.size() == types.size() + 2 && interDirectOtherx.size() == types.size() && maxConnectionsNumberOtherx.size() == types.size())
    {
        res = new CellType(brushx, forcex, interDirectx, sizex, massx, frictionCoeffx, hardnessFactorx, maxConnectionsNumberx);
        for(quint8 i = 0; i < types.size() - 1; i++)
        {
            types[i]->interactDirection.append(interDirectOtherx[i]);
            types[i]->maxConnectionsNumber.insert(types[i]->maxConnectionsNumber.count()-1, maxConnectionsNumberOtherx[i]);
        }
        res->name = namex;
    }
    return res;
}

void CellType::removeType(CellType *id)
{
    for(quint8 i = 0; i < types.size(); i++)
    {
        types[i]->interactDirection.remove(id->number);
        types[i]->maxConnectionsNumber.remove(id->number);
        if(types[i]->number > id->number)
        {
            types[i]->number--;
        }
    }
    types.remove(id->number);
    delete id;
}
