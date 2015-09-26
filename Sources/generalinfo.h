#ifndef GENERALINFO_H
#define GENERALINFO_H

#define M_PI		3.14159265358979323846
#include "Box2D/Box2D.h"

class Textures;

class GeneralInfo
{

public:

    static float D2R (float degrees){
        return degrees * M_PI / 180.0f;
    }
    static float R2D (float radians){
        return radians * 180.0f / M_PI;
    }
    static b2Vec2 mulb2Vec2(b2Vec2 a, b2Vec2 b){
        return b2Vec2(a.x*b.x, a.y*b.y);
    }

    static GeneralInfo& getInstance() {
        static GeneralInfo  p_instance;
        return p_instance;
    }

    enum CollisionType {
       BASIC    =     0x0001,
       BODYPART =     0x0002,
     };


    enum AttackState{
        AS_SWING,
        AS_HIT,
    };

    b2World* world;
    Textures *textures;
    static float deductPeriod(float angle, float period = M_PI * 2.0f);

private:
    static GeneralInfo * p_instance;
    GeneralInfo() {}
    GeneralInfo( const GeneralInfo& );
    GeneralInfo& operator=( GeneralInfo& );

};

#endif // GENERALINFO_H
