#include "ladder.h"

Ladder::Ladder(DisplayData *displayData) : InteractiveObject (displayData){

}

void Ladder::use (Player* player){
    player->isOnLadder = true;
}
