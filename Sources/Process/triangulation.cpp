#include "triangulation.h"

b2Body* Triangulation::triangulateChain(vector<Point*> polyline, b2FixtureDef fixturedef, UserData* UD, b2Vec2 offset, b2BodyType bodyType)
{
    b2World* world=GeneralInfo::getInstance().world;

    b2BodyDef bodydef;
    bodydef.position.Set(offset.x, offset.y);
    bodydef.type = bodyType;

    b2Body* body=world->CreateBody(&bodydef);
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
    body->SetUserData(static_cast<void *>(new UserData));
    return body;
}

vector<Point*> Triangulation::chainToPolyline(b2Fixture* fixture, b2Vec2 scale){
    std::vector <Point*> polyline;

    b2ChainShape* chain = static_cast<b2ChainShape*>( fixture->GetShape());
    int edgeCount = chain->GetChildCount();
    for (int j = 0; j < edgeCount; ++j){
        b2EdgeShape edge;
        ((b2ChainShape*)fixture->GetShape())->GetChildEdge(&edge, j);
        polyline.push_back(new Point(edge.m_vertex2.x * scale.x, edge.m_vertex2.y * scale.y));
    }
    return polyline;
}
vector<Triangle*> Triangulation ::triangulate(std::vector <Point*> polyline){
    CDT* polygon = new CDT(polyline);
    polygon->Triangulate();

    return polygon->GetTriangles();
}

bool Triangulation::isPossiblePolygon(b2Vec2 vertices[], int n){
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

b2Vec2 Triangulation::computePolylineSize(std::vector <Point*> polyline){
    b2Vec2 min = {polyline.at(0)->x, polyline.at(0)->y}, max = min;
    Point point;
    for (int i = 1; i < polyline.size(); i++){
        point = *polyline.at(i);
        if (point.x < min.x)
            min.x = point.x;
        if (point.y < min.y)
            min.y = point.y;

        if (point.x > max.x)
            max.x = point.x;
        if (point.y > max.y)
            max.y = point.y;
    }
    return b2Vec2(max.x - min.x, max.y - min.y);
}

b2Vec2 Triangulation::computeCenter(std::vector <Point*> polyline, float lcX, float lcY){
    b2Vec2 min = {polyline.at(0)->x, polyline.at(0)->y}, max = min;
    Point point;
    for (int i = 1; i < polyline.size(); i++){
        point = *polyline.at(i);
        if (point.x < min.x)
            min.x = point.x;
        if (point.y < min.y)
            min.y = point.y;

        if (point.x > max.x)
            max.x = point.x;
        if (point.y > max.y)
            max.y = point.y;
    }
    return b2Vec2((min.x + max.x) / 2.0f + lcX, (min.y + max.y) / 2.0f + lcY);
}
