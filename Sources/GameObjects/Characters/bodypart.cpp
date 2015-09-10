#include "bodypart.h"

BodyPart::BodyPart(Entity *entity, Type type, b2Body *body, b2RevoluteJoint* RJ) : GameObject (){
    this->type = type;
    this->entity = entity;
    this->body = body;
    this->RJ = RJ;
    desiredAngle = 0;
    motorSpeed = 5;
    angleDeviation = 0.2;
}
