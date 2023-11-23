#include "globalvars.h"

namespace GLOBALVARS
{
    int gravityAcceleration = 10;
    int updateTime = 1;//ms
    //int simulationSpeed = 10;//ms
    //int redrawTime = 33;//ms
    int maxSpeed = -1; //pcs per sec (-1 to disable)
    int boundStiffnessFactor = 3000;
    int maxBoundEnergy = 30000; //(k*dl)/sqrt(sqrt(m1+m2))
    bool enableBounds = false;
    bool enableGravity = false;
    bool enableCollisions = true;
    int enableFriction = 0;
    //int slowModeRait = 1000;//255 - max

    /*qreal force(qreal x, qreal m1, qreal m2)
    {
        //return 0;
        return 1500 *m1*m2 /((x+0.000001)*(x+0.000001));
    }*/
}
