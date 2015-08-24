#include "ladder.h"

Ladder::Ladder(Textures::Texture *texture_p) : InteractiveObject (texture_p){

}

void Ladder::use (Player* player){
    player->isOnLadder = true;
}
