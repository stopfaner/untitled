#include "npc.h"

NPC::NPC(float x, float y) : Entity (x, y){
    moveState = MS_RIGHT;
    isJumping = true;
}

Textures::Type NPC::getTextureType(BodyPart::Type bodyPart)
{
    switch (bodyPart){
    case BodyPart::Type::BODY : return Textures::Type::NPC_BODY;
    case BodyPart::Type::HEAD : return Textures::Type::NPC_HEAD;

    case BodyPart::Type::HIP : return Textures::Type::NPC_HIP;
    case BodyPart::Type::SHIN : return Textures::Type::NPC_SHIN;
    case BodyPart::Type::FOOT: return Textures::Type::NPC_FOOT;

    case BodyPart::Type::FOREARM : return Textures::Type::NPC_FOREARM;
    case BodyPart::Type::SHOULDER: return Textures::Type::NPC_SHOULDER;
    case BodyPart::Type::WRIST: return Textures::Type::NPC_WRIST;
    }
}
