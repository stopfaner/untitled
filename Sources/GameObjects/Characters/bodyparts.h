#ifndef BODYPARTS_H
#define BODYPARTS_H

class b2Body;

class BodyParts
{
public:
    BodyParts(){}
    BodyParts(b2Body* body, b2Body* hip, b2Body* shin, b2Body* foot, b2Body* head);
    b2Body *body, *hip, *shin, *foot, *head;
};

#endif // BODYPARTS_H
