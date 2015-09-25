#ifndef TRIANGULATION_H
#define TRIANGULATION_H
#include <vector>
#include "Sources/UserInterface/displaydata.h"
#include "Sources/UserInterface/keylinedata.h"
#include "Sources/UserInterface/triangletexturedata.h"
#include "Sources/userdata.h"

#include <Box2D/Box2D.h>

#include "poly2tri/poly2tri.h"

using namespace std;
using namespace p2t;

class Triangulation
{
public:
    static Triangulation& getInstance() {
        static Triangulation  p_instance;
        return p_instance;
    }
    static vector<b2Body *> triangulateChain(vector<Point*> polyline, b2FixtureDef fixturedef,
                             UserData *UD, b2Vec2 offset = b2Vec2(0,0), b2BodyType bodyType = b2_dynamicBody);
    static vector<Point *> chainToPolyline(b2Fixture *fixture);
    static vector<Triangle*> triangulate(std::vector<Point *> polyline);
private:
    static bool isPossiblePolygon(b2Vec2 vertices[], int n);

    static Triangulation * p_instance;
    Triangulation() {}
    Triangulation( const Triangulation& );
    Triangulation& operator=( Triangulation& );

};


#endif // TRIANGULATION_H
