#include "displaydata.h"

DisplayData::DisplayData(){
    isShifting = true;
    layer = Layer::NEAREAST;
}

DisplayData::DisplayData(Layer layer) : DisplayData(){
    this->layer = layer;
}

void DisplayData::virtualMethod(){}
