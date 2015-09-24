#ifndef TRIANGULATION_H
#define TRIANGULATION_H
#include <vector>
#include "Sources/UserInterface/displaydata.h"
#include "Sources/UserInterface/keylinedata.h"
#include "Sources/userdata.h"

#include <Box2D/Box2D.h>

#include "poly2tri/poly2tri.h"

using namespace std;
using namespace p2t;

class Triangulation
{
public:

    Triangulation();
    vector<b2Body *> triangulateChain(vector<Point*> polyline, b2FixtureDef fixturedef,
                             UserData *UD, b2Vec2 offset = b2Vec2(0,0), b2BodyType bodyType = b2_dynamicBody);
    vector<Point *> chainToPolyline(b2Fixture *fixture);
    vector<Triangle*> triangulate(std::vector<Point *> polyline);
private:
    bool isPossiblePolygon(b2Vec2 vertices[], int n);
};

#endif // TRIANGULATION_H
