#include "textures.h"
#include <QDebug>
Textures::Textures(){

}

void Textures::loadAll (){
    loadTexture(CRATE, "images/crate.png");
    loadTexture(PLAYER, "images/player.png", 1, 9);
    loadTexture(WALL, "images/wall.png");
    loadTexture(BOT, "images/bot.png");
    loadTexture(BACKGROUND, "images/background.png");
    loadTexture(TEST1, "images/1.png");
    loadTexture(TEST2, "images/2.png");
    loadTexture(TEST3, "images/3.png");
    loadTexture(RUN, "images/run.png", 1, 4);
    loadTexture(JUMP, "images/jump.png", 1, 5);
    loadTexture(VEHICLE, "images/vehicle.png");
    loadTexture(LADDER, "images/ladder.png");
    loadTexture(CLIMBING, "images/climbing.png", 1, 2);
    loadTexture(CLIMBING_IDLE, "images/climbingstop.png");
    loadTexture(GROUND, "images/ground.png");

    loadTexture(BODY, "images/body.png");
    loadTexture(SHIN, "images/shin.png");
    loadTexture(HIP, "images/hip.png");
    loadTexture(SHOULDER, "images/shoulder.png");
    loadTexture(FOREARM, "images/forearm.png");
    loadTexture(FOOT, "images/foot.png");
    loadTexture(WRIST, "images/wrist.png");
    loadTexture(HEAD, "images/head.png");

    loadTexture(NPC_BODY, "images/NPC/body.png");
    loadTexture(NPC_SHIN, "images/NPC/shin.png");
    loadTexture(NPC_HIP, "images/NPC/hip.png");
    loadTexture(NPC_SHOULDER, "images/NPC/shoulder.png");
    loadTexture(NPC_FOREARM, "images/NPC/forearm.png");
    loadTexture(NPC_FOOT, "images/NPC/foot.png");
    loadTexture(NPC_WRIST, "images/NPC/wrist.png");
    loadTexture(NPC_HEAD, "images/NPC/head.png");

    loadTexture(FONT, "images/font.png", 6, 16);

    loadTexture(SWORD, "images/sword.png");
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

Textures::Texture* Textures::getTextureID (Type type){
    for (int i = 0; i < vec.size(); ++i)
        if (vec.at(i)->type == type)
            return vec[i];
    return 0;
}



