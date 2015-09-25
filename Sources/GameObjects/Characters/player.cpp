#include "player.h"

Player::Player(float x, float y) : Entity (x, y){

}

void Player::constructBody(){
    Entity::constructBody();
    weapon = new Sword(bodyParts.wrist2->body, bodyParts.wrist2->body->GetPosition());

}

Textures::Type Player::getTextureType(BodyPart::Type bodyPart)
{
    switch (bodyPart){
    case BodyPart::Type::BODY : return Textures::Type::BODY;
    case BodyPart::Type::HEAD : return Textures::Type::HEAD;

    case BodyPart::Type::HIP : return Textures::Type::HIP;
    case BodyPart::Type::SHIN : return Textures::Type::SHIN;
    case BodyPart::Type::FOOT: return Textures::Type::FOOT;

    case BodyPart::Type::FOREARM : return Textures::Type::FOREARM;
    case BodyPart::Type::SHOULDER: return Textures::Type::SHOULDER;
    case BodyPart::Type::WRIST: return Textures::Type::WRIST;
    }
}

