#ifndef TEXTURE_H
#define TEXTURE_H
#include <SOIL/SOIL.h>
#include <vector>
#include <QGLWidget>

class Textures
{
public:
    enum Type {
        CRATE,
        PLAYER,
        WALL,
        BOT,
        BACKGROUND,
        TEST1, TEST2, TEST3,
        RUN,
        JUMP,
        LADDER,
        CLIMBING
    };
    struct Texture {
        GLuint id;
        Type type;
        int rows = 1;
        int columns = 1;
        int framesQuantity = 1;
    };
    Textures();
    std::vector <Texture*> vec;
    Texture* getTexture (Type);
    void loadAll ();
private:
    void loadTexture (Type, std::string path, int rows = 1, int columns = 1);
};

#endif // TEXTURE_H
