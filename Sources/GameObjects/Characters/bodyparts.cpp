#include "bodyparts.h"

#define M_PI		3.14159265358979323846


float D2R (float degrees);
float R2D (float radians);

BodyParts::BodyParts(){
    body = nullptr;
    hip  = nullptr;
    shin  = nullptr;
    foot  = nullptr;
    head  = nullptr;
    hip2  = nullptr;
    shin2  = nullptr;
    foot2  = nullptr;
    shoulder  = nullptr;
    shoulder2 = nullptr;
    wrist  = nullptr;
    wrist2  = nullptr;
    forearm  = nullptr;
    forearm2 = nullptr;
}
#include "qdebug.h"
void BodyParts::setSpeed(){
    for (std::list<BodyPart*>::const_iterator iterator = list.begin(), end = list.end(); iterator != end; ++iterator) {
        BodyPart* bodyPart = *iterator;
        JointManager::setJointSpeed(bodyPart->RJI);

    }
}

void BodyParts::resetSpeed(){
    for (std::list<BodyPart*>::const_iterator iterator = list.begin(), end = list.end(); iterator != end; ++iterator) {
        BodyPart* bodyPart = *iterator;
        bodyPart->RJI.motorSpeed = bodyPart->RJI.defaultMotorSpeed;
    }
}

void BodyParts::mirror()
{
    for (std::list<BodyPart*>::const_iterator iterator = list.begin(), end = list.end(); iterator != end; ++iterator) {
        BodyPart* bodyPart = *iterator;
        if (bodyPart->RJI.RJ && bodyPart->RJI.RJ->IsLimitEnabled())
            bodyPart->RJI.RJ->SetLimits(- bodyPart->RJI.RJ->GetUpperLimit(), - bodyPart->RJI.RJ->GetLowerLimit());
    }

    BodyPart* foot;
    for (int i = 0; i < 2; ++i){
        if (i)
            foot = this->foot;
        else
            foot = this->foot2;
        foot->RJI.RJ->SetLimits(M_PI + M_PI / 4, 2 * M_PI);
       // foot->body->SetTransform(foot->body->GetPosition(), D2R(180));
    }
}

void BodyParts::setPart(BodyPart *bodyPart){
    switch (bodyPart->type){

    case BodyPart::Type::FOREARM :
        if (!forearm) forearm = bodyPart;
        else forearm2 = bodyPart;
        break;

    case BodyPart::Type::SHOULDER :
        if (!shoulder) shoulder = bodyPart;
        else shoulder2 = bodyPart;
        break;

    case BodyPart::Type::WRIST :
        if (!wrist) wrist = bodyPart;
        else wrist2= bodyPart;
        break;

    case BodyPart::Type::HIP :
        if (!hip) hip= bodyPart;
        else hip2 = bodyPart;
        break;

    case BodyPart::Type::SHIN :
        if (!shin) shin= bodyPart;
        else shin2 = bodyPart;
        break;
    case BodyPart::Type::FOOT :
        if (!foot) foot = bodyPart;
        else foot2= bodyPart;
        break;

    case BodyPart::Type::HEAD :
        head = bodyPart;
        break;

    case BodyPart::Type::BODY :
        body = bodyPart;
        break;

    }
    list.push_back(bodyPart);
}

