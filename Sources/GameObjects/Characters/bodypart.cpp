#include "bodypart.h"

BodyPart::BodyPart(Type type) : UserData (){
    this->type = type;
}

BodyPart::BodyPart(Textures::Texture* texture_p, Type type) : UserData (texture_p)
{
    this->type = type;
}

