#ifndef BODYPARTS_H
#define BODYPARTS_H

class BodyPart;

class BodyParts
{
public:
    BodyParts(){}
    BodyParts(BodyPart* body, BodyPart* head);
    BodyPart *body, *hip, *shin, *foot, *head, *hip2, *shin2, *foot2, *shoulder, *shoulder2, *wrist, *wrist2, *forearm, *forearm2;
};

#endif // BODYPARTS_H
