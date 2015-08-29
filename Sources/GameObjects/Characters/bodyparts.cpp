#include "bodyparts.h"

BodyParts::BodyParts(b2Body *body, b2Body *hip, b2Body *shin, b2Body *foot, b2Body *head)
{
    this->body = body;
    this->hip = hip;
    this->shin = shin;
    this->foot = foot;
    this->head = head;
}

