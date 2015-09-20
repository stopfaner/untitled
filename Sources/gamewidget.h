/*
 * Copyright 2014 untitled project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>

#include <Box2D/Box2D.h>
#include "Box2D/Json/b2dJson.h"

#include <SOIL.h>
#include <vector>
#include <time.h>

#include "GameObjects/Characters/npc.h"
#include "GameObjects/Characters/bot.h"
#include "GameObjects/Characters/player.h"
#include "GameObjects/Characters/bodypart.h"
#include "GameObjects/enum.h"
#include "GameObjects/entitylist.h"

#include "GameObjects/gameobject.h"
#include "GameObjects/InteractiveObjects/ladder.h"
#include "GameObjects/InteractiveObjects/Vehicles/car.h"
#include "GameObjects/InteractiveObjects/Vehicles/car.h"

#include "GameObjects/Builds/build.h"

#include "UserInterface/textures.h"
#include "UserInterface/color.h"
#include "UserInterface/displaydata.h"
#include "UserInterface/keylinedata.h"
#include "UserInterface/texturedata.h"
#include "UserInterface/nondrawable.h"
#include "UserInterface/HUD/hudelement.h"
#include "UserInterface/triangletexturedata.h"

#include "Clipper/clipper.hpp"

#include "generalinfo.h"
#include "userdata.h"

#include "poly2tri/poly2tri.h"

/**
 * @brief The GameWidget class describes main window activity
 */
using namespace std;
using namespace p2t;

struct Triangle{
   b2Vec2 p1;
   b2Vec2 p2;
   b2Vec2 p3;
};

class GameWidget : public QGLWidget {

public:

     /**
     * @brief GameWidget - constructor
     * @param parent - parent widget instance
     */
    GameWidget(QWidget *parent = 0);
//public slots:
    void updateGame();
    void addWalkingMachine();
    void drawRectangle(b2Vec2 center, float width, float height, float angle, TextureData *textureData);
    void loadBackground();
private:
    int WIDTH = 600;
    int HEIGHT = 600;

    float M2P = 40;
    float P2M=1/M2P;

    //zoom koef
    float kx = 0.7;
    float ky = 0.7;

    Player *player;


    QTimer *timer;

    b2World* world;

    void initializeGL();

    void resizeGL(int nWidth, int nHeight);

    void paintGL();

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    b2Body *addRect(b2Vec2 center, float w, float h, bool dyn, Textures::Type type, bool isSensor = false);
    b2Body* addRect(float x, float y, float width, float height, bool dyn, Textures::Type type, bool isSensor = false);

    b2Body* addSpecRect ();

    void triangulateChain(b2dJson &json);
    void drawTriangle(b2Vec2 *points, int count, b2Vec2 center, float angle, TriangleTextureData *triangleTextureData);
    void drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, TextureData *textureData);
    void drawPolygon(b2Vec2* points, int count, b2Vec2 center,float angle, KeyLineData *keyLineData);
    void drawCircle(float radius, b2Vec2 center, KeyLineData *keyLineData, float angle);
    void drawChain(b2Vec2* points, b2Vec2 center, int count, KeyLineData *keyLineData);

    vector<Triangle> triangulate(std::vector<Point *> polyline);

    void createWorld();

    b2Body *addBot(Bot* bot);

    Textures *textures;

    vector<b2Body*> destroyBodies;
    std::list<UIElement*> displayItems;
    void destroyLandscape();
};

#endif // GAMEWIDGET_H
