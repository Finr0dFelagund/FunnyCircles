#include "celltickprocesser.h"

CellTickProcesser::CellTickProcesser(QVector<Cell*> *cellArray_, QVector<QVector<quint16>>* connectionsIndArray_)
    :cellArray(cellArray_), connectionsIndArray(connectionsIndArray_)
{;}

void CellTickProcesser::calculate(QThread* thr, quint16 startInd, quint16 endInd)
{
    if(thr == QThread::currentThread())
    {
        /*if(GLOBALVARS::enableBounds)
        {
            createConnections();
        }
        else
        {
            while(connectionsInd[0].size() > 0)
            {
                removeConnect(0);
            }
         }*/
        calcForces(startInd, endInd);
        emit calculationReady(QThread::currentThread());
    }
}

void CellTickProcesser::applyDeltas(QThread* thr, quint16 startInd, quint16 endInd)
{
    if(thr == QThread::currentThread())
    {
        for(quint16 i = startInd; i < endInd; i++)
        {
            (*cellArray)[i]->acceleration = (*cellArray)[i]->sumForce / (*cellArray)[i]->type->mass;
            (*cellArray)[i]->speed += (*cellArray)[i]->acceleration * GLOBALVARS::updateTime/1000;
            if(GLOBALVARS::maxSpeed != -1 && (*cellArray)[i]->speed.length() > GLOBALVARS::maxSpeed) (*cellArray)[i]->speed = (*cellArray)[i]->speed.normalized() * GLOBALVARS::maxSpeed;
            if((*cellArray)[i]->moveAble) (*cellArray)[i]->position += (*cellArray)[i]->speed*GLOBALVARS::updateTime/1000;
        }
        emit applyingDeltasReady(QThread::currentThread());
    }
}

void CellTickProcesser::createConnections(quint16 startInd, quint16 endInd)
{
    for(quint16 i = startInd; i < endInd; i++)
    {
        for(quint16 j = startInd + 1; j < (*cellArray).size(); j++)
        {
            tryToConnect(i, j);
        }
    }
}

bool CellTickProcesser::tryToConnect(quint16 a, quint16 b)
{
    bool result = false;
    if(GLOBALVARS::enableBounds)
    {
        Cell *consumer = (*cellArray)[a], *dealer = (*cellArray)[b];
        qreal distance = consumer->position.distanceToPoint(dealer->position) - (consumer->type->size + dealer->type->size)/2;
        qreal boundEnergy = GLOBALVARS::boundStiffnessFactor * ((distance > 0) ? distance : 0) / sqrt(sqrt(consumer->type->mass + dealer->type->mass));
        if(a!=b && boundEnergy < GLOBALVARS::maxBoundEnergy)
        {
            for(quint16 i = 0; i < (*connectionsIndArray)[0].size(); i++)
            {
                if(((*connectionsIndArray)[0][i] == a && (*connectionsIndArray)[1][i] == b) || ((*connectionsIndArray)[0][i] == b && (*connectionsIndArray)[1][i] == a))
                    return false;
            }
            if((consumer->numberOfTypeConnections[dealer->type->number] < consumer->type->maxConnectionsNumber[dealer->type->number] || consumer->type->maxConnectionsNumber[dealer->type->number] < 0) &&
                (dealer->numberOfTypeConnections[consumer->type->number] < dealer->type->maxConnectionsNumber[consumer->type->number] || dealer->type->maxConnectionsNumber[consumer->type->number] < 0) &&
                (consumer->numberOfTypeConnections.last() < consumer->type->maxConnectionsNumber.last() || consumer->type->maxConnectionsNumber.last() < 0) &&
                (dealer->numberOfTypeConnections.last() < dealer->type->maxConnectionsNumber.last() || dealer->type->maxConnectionsNumber.last() < 0))
            {
                (*connectionsIndArray)[0].append(a);
                (*connectionsIndArray)[1].append(b);
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

void CellTickProcesser::removeConnect(quint16 ind)//ind - index of bound
{
    (*cellArray)[(*connectionsIndArray)[0][ind]]->numberOfTypeConnections[(*cellArray)[(*connectionsIndArray)[1][ind]]->type->number]--;
    (*cellArray)[(*connectionsIndArray)[0][ind]]->numberOfTypeConnections.last()--;
    (*cellArray)[(*connectionsIndArray)[1][ind]]->numberOfTypeConnections[(*cellArray)[(*connectionsIndArray)[0][ind]]->type->number]--;
    (*cellArray)[(*connectionsIndArray)[1][ind]]->numberOfTypeConnections.last()--;
    (*connectionsIndArray)[0].remove(ind);
    (*connectionsIndArray)[1].remove(ind);
}

void CellTickProcesser::calcForces(quint16 startInd, quint16 endInd)
{
    QVector2D routeJtoI;
    qreal distance;
    Cell *dealer, *consumer;
    for(quint16 i = startInd; i < endInd; i++)
    {
        (*cellArray)[i]->sumForce.setX(0);
        (*cellArray)[i]->sumForce.setY(0);
    }
    if(GLOBALVARS::enableGravity)//gravity forces
    {
        QVector2D fieldForce;
        for(quint16 i = startInd; i < endInd; i++)
        {
            consumer = (*cellArray)[i];
            for(quint16 j = 0; j < (*cellArray).size(); j++)
            {
                dealer = (*cellArray)[j];
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
        for(quint16 i = startInd; i < endInd; i++)
        {
            (*cellArray)[i]->sumForce += (*cellArray)[i]->speed.normalized() * (-1) * (*cellArray)[i]->type->mass * (*cellArray)[i]->type->frictionCoeff * GLOBALVARS::gravityAcceleration;
        }
    }
    /*if(GLOBALVARS::enableBounds)//bound forces
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
    }*/
    if(GLOBALVARS::enableCollisions)//CollisionForces THREAD UNSAFETY
    {
        qreal elongation;
        QVector2D elasticForce;
        for(quint16 i = startInd; i < endInd; i++)
        {
            consumer = (*cellArray)[i];
            for(quint16 j = startInd + 1; j < (*cellArray).size(); j++)
            {
                dealer = (*cellArray)[j];
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
