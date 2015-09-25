#include "displaydata.h"

DisplayData::DisplayData(){
    isShifting = true;
    isVisible = true;
    layer = Layer::NEAREST;
}

DisplayData::DisplayData(Layer layer) : DisplayData(){
    this->layer = layer;
}

void DisplayData::virtualMethod(){}
