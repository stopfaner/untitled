#ifndef JOINTMANAGER_H
#define JOINTMANAGER_H
#include <Box2D/Box2D.h>

struct RevoluteJointInfo{
    b2RevoluteJoint *RJ;
    float desiredAngle;
    float angleDeviation;
    float motorSpeed;
    float defaultMotorSpeed;
};

class JointManager
{
public:
    JointManager();
    static void setJointSpeed(RevoluteJointInfo RJI);
};

#endif // JOINTMANAGER_H
