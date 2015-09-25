#include "triangulation.h"

vector<b2Body*> Triangulation::triangulateChain(vector<Point*> polyline, b2FixtureDef fixturedef, UserData* UD, b2Vec2 offset, b2BodyType bodyType)
{
    b2World* world=GeneralInfo::getInstance().world;
    vector <b2Body*> triangleBodies;


    b2BodyDef bodydef;
    bodydef.position.Set(offset.x, offset.y);
    bodydef.type = bodyType;

    b2Body* body=world->CreateBody(&bodydef);
    triangleBodies.push_back(body);
    b2PolygonShape shape;

    vector<Triangle*> triangles = triangulate(polyline);
    for (int j = 0; j < triangles.size(); ++j){
        Triangle* triangle = triangles.at(j);
        b2Vec2 points[3];
        for (unsigned int k = 0; k < 3; ++k){
            points[k].x = triangle->GetPoint(k)->x;
            points[k].y = triangle->GetPoint(k)->y;
        }
        if (isPossiblePolygon(points, 3)){
            shape.Set(points, 3);
            fixturedef.shape = &shape;

            b2Fixture* fixture = body->CreateFixture(&fixturedef);

            fixture->SetUserData(static_cast<void*>(UD));
        }
    }
    body->ResetMassData();

    return triangleBodies;
}

vector<Point*> Triangulation::chainToPolyline(b2Fixture* fixture){
    std::vector <Point*> polyline;

    b2ChainShape* chain = static_cast<b2ChainShape*>( fixture->GetShape());
    int edgeCount = chain->GetChildCount();
    for (int j = 0; j < edgeCount; ++j){
        b2EdgeShape edge;
        ((b2ChainShape*)fixture->GetShape())->GetChildEdge(&edge, j);
        polyline.push_back(new Point(edge.m_vertex2.x, edge.m_vertex2.y));
    }
    return polyline;
}
vector<Triangle*> Triangulation ::triangulate(std::vector <Point*> polyline){
    CDT* polygon = new CDT(polyline);
    polygon->Triangulate();

    return polygon->GetTriangles();
}

bool Triangulation ::isPossiblePolygon(b2Vec2 vertices[], int n){
    b2Vec2 ps[b2_maxPolygonVertices];
    int32 tempCount = 0;
    for (int32 i = 0; i < n; ++i)
    {
        b2Vec2 v = vertices[i];

        bool unique = true;
        for (int32 j = 0; j < tempCount; ++j)
        {
            if (b2DistanceSquared(v, ps[j]) < 0.5f * b2_linearSlop)
            {
                unique = false;
                break;
            }
        }

        if (unique)
        {
            ps[tempCount++] = v;
        }
    }

    n = tempCount;
    if (n < 3)
        return false;
    return true;
}
