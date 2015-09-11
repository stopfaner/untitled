#include "vehicle.h"

Vehicle::Vehicle() : InteractiveObject (){
    this->world = GeneralInfo::getInstance().world;
}
