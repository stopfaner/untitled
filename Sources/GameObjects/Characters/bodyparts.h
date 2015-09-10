#ifndef BODYPARTS_H
#define BODYPARTS_H

#include <list>

#include "bodypart.h"
#include "Box2D/Box2D.h"

class BodyParts
{
public:
    BodyParts();
    void setPart (BodyPart* bodyPart);
    void setSpeed ();
    void mirror ();
    std::list<BodyPart*> list;

    BodyPart *body, *hip, *shin, *foot, *head, *hip2, *shin2, *foot2, *shoulder, *shoulder2,
        *wrist, *wrist2, *forearm, *forearm2;
};

#endif // BODYPARTS_H
