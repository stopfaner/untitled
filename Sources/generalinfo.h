#ifndef GENERALINFO_H
#define GENERALINFO_H

#define M_PI		3.14159265358979323846

class b2World;
class Textures;
class b2Vec2;

class GeneralInfo
{

public:
    static GeneralInfo& getInstance() {
        static GeneralInfo  p_instance;
        return p_instance;
    }

    enum CollisionType {
       BASIC    =     0x0001,
       BODYPART =     0x0002,
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
