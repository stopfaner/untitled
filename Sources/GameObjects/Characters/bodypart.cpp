#include "bodypart.h"

void BodyPart::setDefaultSpeed(float speed)
{
    RJI.defaultMotorSpeed = speed;
    RJI.motorSpeed = speed;
}

BodyPart::BodyPart(Entity *entity, Type type, b2Body *body, b2RevoluteJoint* RJ) : GameObject (){
    this->type = type;
    this->entity = entity;
    this->body = body;
    RJI.RJ = RJ;
    RJI.desiredAngle = 0;
    RJI.defaultMotorSpeed = 5;
    RJI.motorSpeed = RJI.defaultMotorSpeed;
    RJI.angleDeviation = 0.2;
}
