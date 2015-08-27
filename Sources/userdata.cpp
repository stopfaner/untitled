#include "userdata.h"

UserData::UserData(){
    gameObject = nullptr;
    displayData = nullptr;
}

UserData::UserData(DisplayData* displayData){
    gameObject = nullptr;
    this->displayData = displayData;
}

UserData::UserData(GameObject *gameObject, DisplayData *displayData){
    this->gameObject = gameObject;
    this->displayData = displayData;
}

