#include "textures.h"
#include <QDebug>
Textures::Textures(){

}

void Textures::loadAll (){
    loadTexture(CRATE, "crate.png");
    loadTexture(PLAYER, "player.png", 1, 9);
    loadTexture(WALL, "wall.png");
    loadTexture(BOT, "bot.png");
    loadTexture(BACKGROUND, "background.png");
    loadTexture(TEST1, "1.png");
    loadTexture(TEST2, "2.png");
    loadTexture(TEST3, "3.png");
    loadTexture(RUN, "run.png", 1, 4);
    loadTexture(JUMP, "jump.png", 1, 5);
    loadTexture(LADDER, "ladder.png");
    loadTexture(CLIMBING, "climbing.png", 1, 2);
    loadTexture(CLIMBING_STOP, "climbingstop.png");
}

void Textures::loadTexture (Type type, std::string path, int rows, int columns){

    Texture* tex = new Texture;
    tex->rows = rows;
    tex->columns = columns;
    tex->type = type;
    tex->framesQuantity = columns * rows;

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    tex->id = SOIL_load_OGL_texture(
                path.c_str(),
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    
    vec.push_back(tex);
}

Textures::Texture* Textures::getTexture (Type type){
    for (int i = 0; i < vec.size(); ++i)
        if (vec.at(i)->type == type)
            return vec[i];
}



