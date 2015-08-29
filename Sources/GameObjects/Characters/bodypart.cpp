#include "bodypart.h"

BodyPart::BodyPart(Entity *entity, Type type, b2Body *body) : GameObject (){
    this->type = type;
    this->entity = entity;
    this->body = body;
}
