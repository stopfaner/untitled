#include "userdata.h"

UserData::UserData(){
    isMirrored = false;
    isPlayer = false;
}

UserData::UserData(Textures::Type type){
    UserData();
    textureType = type;
}
