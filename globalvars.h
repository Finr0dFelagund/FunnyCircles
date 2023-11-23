#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <QtCore>

namespace GLOBALVARS
{
    extern int gravityAcceleration;
    extern int updateTime;
    //extern int simulationSpeed;
    //extern int redrawTime;
    extern int maxSpeed;
    extern int boundStiffnessFactor;
    extern int maxBoundEnergy;
    extern bool enableBounds;
    extern bool enableGravity;
    extern bool enableCollisions;
    extern int enableFriction;
    //extern int slowModeRait;

    //extern qreal force(qreal x, qreal m1, qreal m2);
}

#endif // GLOBALVARS_H
