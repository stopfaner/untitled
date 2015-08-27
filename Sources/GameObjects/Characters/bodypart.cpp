#include "bodypart.h"

BodyPart::BodyPart(Entity *entity, Type type) : GameObject (){
    this->type = type;
    this->entity = entity;
}
