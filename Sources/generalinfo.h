#ifndef GENERALINFO_H
#define GENERALINFO_H

class b2World;
class Textures;

class GeneralInfo
{

public:
    static GeneralInfo& getInstance() {
        static GeneralInfo  p_instance;
        return p_instance;
    }

    b2World* world;
    Textures *textures;

    enum CollisionType {
       BASIC =          0x0001,
       BODYPART =     0x0002,
     };

private:
    static GeneralInfo * p_instance;
    GeneralInfo() {}
    GeneralInfo( const GeneralInfo& );
    GeneralInfo& operator=( GeneralInfo& );
};

#endif // GENERALINFO_H