#include "cells.h"

Cells::Cells()
{
    connectionsInd = QVector<QVector<quint16>>(2);
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
    for(quint16 i = 0; i < connectionsInd[0].size(); i++)
    {
        if(connectionsInd[0][i] > id)
        {
            connectionsInd[0][i]--;
        }
        if(connectionsInd[1][i] > id)
        {
            connectionsInd[1][i]--;
        }
    }
}

void Cells::removeConnect(quint16 ind)//ind - index of bound
{
    cells[connectionsInd[0][ind]]->numberOfTypeConnections[cells[connectionsInd[1][ind]]->type->number]--;
    cells[connectionsInd[0][ind]]->numberOfTypeConnections.last()--;
    cells[connectionsInd[1][ind]]->numberOfTypeConnections[cells[connectionsInd[0][ind]]->type->number]--;
    cells[connectionsInd[1][ind]]->numberOfTypeConnections.last()--;
    connectionsInd[0].remove(ind);
    connectionsInd[1].remove(ind);
}

void Cells::removeAllConnections(quint16 ind)//ind - ind of cell
{
    for(quint16 i = 0; i < connectionsInd[0].size(); i++)
    {
        if(connectionsInd[0][i] == ind || connectionsInd[1][i] == ind)
        {
            removeConnect(i);
            i--;
        }
    }
}

void Cells::removeConnect(quint16 a, quint16 b)
{
    for(quint16 i = 0; i < connectionsInd[0].size(); i++)
    {
        if((connectionsInd[0][i] == b && connectionsInd[1][i] == a) || (connectionsInd[0][i] == a && connectionsInd[1][i] == b))
        {
            removeConnect(i);
            break;
        }
    }
}

bool Cells::tryToConnect(quint16 a, quint16 b)
{
    bool result = false;
    if(GLOBALVARS::enableBounds)
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
    }
    return result;
}

void Cells::createConnections()
{
    for(quint16 i = 0; i < cells.size() - 1; i++)
    {
        for(quint16 j = i + 1; j < cells.size(); j++)
        {
            tryToConnect(i, j);
        }
    }
}

void Cells::calcForces()
{
    QVector2D routeJtoI;
    qreal distance;
    Cell *dealer, *consumer;
    for(quint16 i = 0; i < cells.size(); i++)
    {
        cells[i]->sumForce.setX(0);
        cells[i]->sumForce.setY(0);
    }
    if(GLOBALVARS::enableGravity)//gravity forces
    {
        QVector2D fieldForce;
        for(quint16 i = 0; i < cells.size(); i++)
        {
            consumer = cells[i];
            for(quint16 j = 0; j < cells.size(); j++)
            {
                dealer = cells[j];
                if(consumer != dealer)
                {
                    routeJtoI = (consumer->position - dealer->position).normalized();
                    distance = consumer->position.distanceToPoint(dealer->position);
                    fieldForce = routeJtoI * dealer->type->force(distance, consumer->type->mass, dealer->type->mass) *
                            dealer->type->interactDirection[consumer->type->number]/**((distance > (consumer->type->size + dealer->type->size)/2) ? 1 : 0)*/;//getting force from field of dealer
                    consumer->sumForce += fieldForce;
                }
            }
        }
    }
    if(GLOBALVARS::enableFriction)//friction forces
    {
        for(quint16 i = 0; i < cells.size(); i++)
        {
            cells[i]->sumForce += cells[i]->speed.normalized() * (-1) * cells[i]->type->mass * cells[i]->type->frictionCoeff * GLOBALVARS::gravityAcceleration;
        }
    }
    if(GLOBALVARS::enableBounds)//bound forces
    {
        QVector2D route0to1, boundForse;
        qreal distance, boundEnergy;
        for(qint16 i = 0; i < connectionsInd[0].size(); i++)
        {
            route0to1 = (cells[connectionsInd[1][i]]->position - cells[connectionsInd[0][i]]->position).normalized();
            distance = cells[connectionsInd[0][i]]->position.distanceToPoint(cells[connectionsInd[1][i]]->position) -
                       (cells[connectionsInd[0][i]]->type->size + cells[connectionsInd[1][i]]->type->size)/2;
            distance = distance > 0 ? distance : 0;
            boundForse = route0to1 * distance * GLOBALVARS::boundStiffnessFactor;
            boundEnergy = GLOBALVARS::boundStiffnessFactor * distance / sqrt(sqrt(cells[connectionsInd[0][i]]->type->mass + cells[connectionsInd[1][i]]->type->mass));
            if(boundEnergy < GLOBALVARS::maxBoundEnergy)
            {
                cells[connectionsInd[0][i]]->sumForce += boundForse;
                cells[connectionsInd[1][i]]->sumForce -= boundForse;
            }
            else
            {
                removeConnect(i);
                i--;
            }
        }
    }
    if(GLOBALVARS::enableCollisions)//CollisionForces
    {
        qreal elongation;
        QVector2D elasticForce;
        for(quint16 i = 0; i < cells.size() - 1; i++)
        {
            consumer = cells[i];
            for(quint16 j = i + 1; j < cells.size(); j++)
            {
                dealer = cells[j];
                if(consumer->collisionAble && dealer->collisionAble)
                {
                    routeJtoI = (consumer->position - dealer->position).normalized();
                    distance = consumer->position.distanceToPoint(dealer->position);
                    elongation = (consumer->type->hardnessFactor == 0 || dealer->type->hardnessFactor == 0 || distance > (consumer->type->size + dealer->type->size)/2 ) ? 0 :
                                     ((consumer->type->size + dealer->type->size)/2 - distance) / (dealer->type->hardnessFactor/consumer->type->hardnessFactor + 1);
                    elasticForce = pow(elongation, 4) * dealer->type->hardnessFactor * routeJtoI;
                    consumer->sumForce += elasticForce;
                    elongation *= (dealer->type->hardnessFactor/consumer->type->hardnessFactor);
                    elasticForce = pow(elongation, 4) * consumer->type->hardnessFactor * (-1) * routeJtoI;
                    dealer->sumForce += elasticForce;
                }
            }
        }
    }
}

inline void Cells::applyDeltas()
{
    for(quint16 i = 0; i < cells.size(); i++)
    {
        cells[i]->acceleration = cells[i]->sumForce / cells[i]->type->mass;
        cells[i]->speed += cells[i]->acceleration * GLOBALVARS::updateTime/1000;
        if(GLOBALVARS::maxSpeed != -1 && cells[i]->speed.length() > GLOBALVARS::maxSpeed) cells[i]->speed = cells[i]->speed.normalized() * GLOBALVARS::maxSpeed;
        if(cells[i]->moveAble) cells[i]->position += cells[i]->speed*GLOBALVARS::updateTime/1000;
    }
}

void Cells::onTick(quint8 numOfTicks)
{
    for(quint8 i = 0; i < numOfTicks; i++)
    {
        if(GLOBALVARS::enableBounds)
        {
            createConnections();
        }
        else
        {
            while(connectionsInd[0].size() > 0)
            {
                removeConnect(0);
            }
        }
        calcForces();
        applyDeltas();
    }
}
