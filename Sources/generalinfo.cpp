#include "generalinfo.h"

float GeneralInfo::deductPeriod(float angle, float period){
    return (angle - static_cast<int>(angle / period) * period);
}
