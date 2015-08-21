#include "userdata.h"

UserData::UserData(){
    isMirrored = false;
}

UserData::UserData(Textures::Type type){
    UserData();
    textureType = type;
}
