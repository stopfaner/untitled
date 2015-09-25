#include "jointmanager.h"

void JointManager::setJointSpeed(RevoluteJointInfo RJI)
{
    if (RJI.RJ)
        if ( RJI.RJ->GetJointAngle() - RJI.desiredAngle > RJI.angleDeviation)
           RJI.RJ->SetMotorSpeed(-RJI.motorSpeed);
        else
            if ( RJI.RJ->GetJointAngle() - RJI.desiredAngle < - RJI.angleDeviation)
                RJI.RJ->SetMotorSpeed(RJI.motorSpeed);
            else RJI.RJ->SetMotorSpeed(0);
}

JointManager::JointManager()
{

}

