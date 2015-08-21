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
        TEST1, TEST2, TEST3
    };
    struct Texture {
        GLuint id;
        Type type;
    };
    Textures();
    std::vector <Texture> vec;
    GLuint getTextureId (Type);
    void loadAll ();
private:
    void loadTexture (Type);
};

#endif // TEXTURE_H
