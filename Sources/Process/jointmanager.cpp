#include "jointmanager.h"

void JointManager::setJointSpeed(RevoluteJointInfo RJI)
{
    if (RJI.RJ){
        float angle = RJI.RJ->GetJointAngle();
        if ( angle - RJI.desiredAngle > RJI.angleDeviation)
           RJI.RJ->SetMotorSpeed(-RJI.motorSpeed);
        else
            if ( angle - RJI.desiredAngle < - RJI.angleDeviation)
                RJI.RJ->SetMotorSpeed(RJI.motorSpeed);
            else RJI.RJ->SetMotorSpeed(0);
    }
}

JointManager::JointManager()
{

}

