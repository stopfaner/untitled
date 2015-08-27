#include "ladder.h"

Ladder::Ladder() : InteractiveObject (){

}

void Ladder::use (Player* player){
    player->isOnLadder = true;
}
