#include "displaydata.h"

DisplayData::DisplayData(){
    isShifting = true;
    layer = Layer::NEAREST;
}

DisplayData::DisplayData(Layer layer) : DisplayData(){
    this->layer = layer;
}

void DisplayData::virtualMethod(){}
