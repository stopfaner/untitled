#include "bodypart.h"

BodyPart::BodyPart(DisplayData* displayData, Type type) : UserData (displayData){
    this->type = type;
}
