#include "textures.h"

Textures::Textures(){

}

void Textures::loadAll (){
    loadTexture(CRATE);
    loadTexture(PLAYER);
    loadTexture(WALL);
    loadTexture(TEST1);
    loadTexture(TEST2);
    loadTexture(TEST3);
}

void Textures::loadTexture (Type type){
    Texture tex;
    tex.type = type;
    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);
    std::string path;
    switch (type){
    case CRATE: path = "crate.png"; break;
    case PLAYER: path = "player.png"; break;
    case WALL: path = "wall.png"; break;
    case TEST1: path = "1.png"; break;
    case TEST2: path = "2.png"; break;
    case TEST3: path = "3.png"; break;
    }
    tex.id = SOIL_load_OGL_texture(
                path.c_str(),
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    vec.push_back(tex);
}

GLuint Textures::getTextureId (Type type){
    for (int i = 0; i < vec.size(); ++i){
        if (vec.at(i).type == type) return vec[i].id;
    }
}



