#include "celltickprocesser.h"
#include "globalvars.h"

CellTickProcesser::CellTickProcesser(QVector<Cell*> *cellArray_)
    :cellArray(cellArray_)
{;}

void CellTickProcesser::calculate(QThread* thr, quint16 startInd, quint16 endInd)
{
    if(thr == QThread::currentThread())
    {
        if(GLOBALVARS::enableBounds)
        {
            createConnections(startInd, endInd);
        }
        else
        {
            removeConnections(startInd, endInd);
         }
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
    Cell *consumer, *dealer;
    qreal distance, boundEnergy;
    bool flag;
    for(quint16 i = startInd; i < endInd; i++)
    {
        for(quint16 j = 0; j < (*cellArray).size(); j++)
        {
            if(GLOBALVARS::enableBounds)
            {
                flag = false;
                consumer = (*cellArray)[i];
                dealer = (*cellArray)[j];
                distance = consumer->position.distanceToPoint(dealer->position) - (consumer->type->size + dealer->type->size)/2;
                boundEnergy = GLOBALVARS::boundStiffnessFactor * ((distance > 0) ? distance : 0) / sqrt(sqrt(consumer->type->mass + dealer->type->mass));
                if(consumer != dealer && boundEnergy < GLOBALVARS::maxBoundEnergy)
                {
                    for(quint16 k = 0; k < consumer->connectionsInd.size(); k++)
                    {
                        if(consumer->connectionsInd[k] == j)
                        {
                            flag = true;
                            break;
                        }
                    }
                    if(flag)
                    {
                        continue;
                    }
                    else
                    {
                        (*cellArray)[i <= j ? i : j]->mutex.lock();
                        (*cellArray)[i <= j ? j : i]->mutex.lock();
                        if((consumer->numberOfTypeConnections[dealer->type->number] < consumer->type->maxConnectionsNumber[dealer->type->number] || consumer->type->maxConnectionsNumber[dealer->type->number] < 0) &&
                            (dealer->numberOfTypeConnections[consumer->type->number] < dealer->type->maxConnectionsNumber[consumer->type->number] || dealer->type->maxConnectionsNumber[consumer->type->number] < 0) &&
                            (consumer->numberOfTypeConnections.last() < consumer->type->maxConnectionsNumber.last() || consumer->type->maxConnectionsNumber.last() < 0) &&
                            (dealer->numberOfTypeConnections.last() < dealer->type->maxConnectionsNumber.last() || dealer->type->maxConnectionsNumber.last() < 0))
                        {
                            consumer->connectionsInd.append(j);
                            consumer->numberOfTypeConnections[dealer->type->number]++;
                            consumer->numberOfTypeConnections.last()++;
                            dealer->connectionsInd.append(i);
                            dealer->numberOfTypeConnections[consumer->type->number]++;
                            dealer->numberOfTypeConnections.last()++;
                        }
                        (*cellArray)[i <= j ? j : i]->mutex.unlock();
                        (*cellArray)[i <= j ? i : j]->mutex.unlock();
                    }
                }
            }
        }
    }
}

void CellTickProcesser::removeConnections(quint16 startInd, quint16 endInd)
{
    for(quint16 i = startInd; i < endInd; i++)
    {
        (*cellArray)[i]->connectionsInd.clear();
        for(int j = 0; j < (*cellArray)[i]->numberOfTypeConnections.size(); j++)
        {
            (*cellArray)[i]->numberOfTypeConnections[j] = 0;
        }
    }
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
                                 dealer->type->interactDirection[consumer->type->number];//getting force from field of dealer
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
    if(GLOBALVARS::enableBounds)//bound forces
    {
        QVector2D route0to1, route1to0, boundForse;
        qreal distance, boundEnergy;
        for(qint16 i = startInd; i < endInd; i++)
        {
            for(int j = 0; j < (*cellArray)[i]->connectionsInd.size(); j++)
            {
                route0to1 = ((*cellArray)[(*cellArray)[i]->connectionsInd[j]]->position - (*cellArray)[i]->position).normalized();
                distance = (*cellArray)[(*cellArray)[i]->connectionsInd[j]]->position.distanceToPoint((*cellArray)[i]->position) -
                           ((*cellArray)[(*cellArray)[i]->connectionsInd[j]]->type->size + (*cellArray)[i]->type->size)/2;
                boundForse = route0to1 * distance * GLOBALVARS::boundStiffnessFactor;
                boundEnergy = GLOBALVARS::boundStiffnessFactor * distance /
                              sqrt(sqrt((*cellArray)[(*cellArray)[i]->connectionsInd[j]]->type->mass + (*cellArray)[i]->type->mass));
                if(boundEnergy < GLOBALVARS::maxBoundEnergy)
                {
                    (*cellArray)[i]->sumForce += boundForse;
                }
                else
                {
                    (*cellArray)[i]->numberOfTypeConnections[(*cellArray)[(*cellArray)[i]->connectionsInd[j]]->type->number]--;
                    (*cellArray)[i]->numberOfTypeConnections.last()--;
                    (*cellArray)[i]->connectionsInd.remove(j);
                    j--;
                }
            }
        }
    }
    if(GLOBALVARS::enableCollisions)//CollisionForces
    {
        qreal elongation;
        QVector2D elasticForce;
        for(quint16 i = startInd; i < endInd; i++)
        {
            consumer = (*cellArray)[i];
            for(quint16 j = 0; j < (*cellArray).size(); j++)
            {
                dealer = (*cellArray)[j];
                if(consumer->collisionAble && dealer->collisionAble)
                {
                    routeJtoI = (consumer->position - dealer->position).normalized();
                    distance = consumer->position.distanceToPoint(dealer->position);
                    elongation = (consumer->type->hardnessFactor == 0 || dealer->type->hardnessFactor == 0 || distance > (consumer->type->size + dealer->type->size)/2 ) ? 0 :
                                     ((consumer->type->size + dealer->type->size)/2 - distance) / (consumer->type->hardnessFactor/dealer->type->hardnessFactor + 1);
                    elasticForce = pow(elongation, 4) * dealer->type->hardnessFactor * routeJtoI;
                    consumer->sumForce += elasticForce;
                }
            }
        }
    }
}
