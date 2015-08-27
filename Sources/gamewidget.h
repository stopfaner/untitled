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

#include <SOIL.h>
#include <vector>
#include <time.h>

#include "Sources/UserInterface/textures.h"
#include "GameObjects/Characters/bot.h"
#include "GameObjects/Characters/player.h"
#include "GameObjects/Characters/bodypart.h"
#include "GameObjects/Rooms/room.h"
#include "GameObjects/userdata.h"
#include "GameObjects/InteractiveObjects/ladder.h"
#include "GameObjects/InteractiveObjects/Vehicles/car.h"
#include "UserInterface/color.h"
#include "UserInterface/displaydata.h"
#include "UserInterface/keylinedata.h"
#include "UserInterface/texturedata.h"
#include "UserInterface/nondrawable.h"

/**
 * @brief The GameWidget class describes main window activity
 */
using namespace std;



class GameWidget : public QGLWidget {

public:
    /**
     * @brief GameWidget - constructor
     * @param parent - parent widget instance
     */
    GameWidget(QWidget *parent = 0);
//public slots:
    void updateGame();
private:
    int WIDTH = 600;
    int HEIGHT = 600;

    float M2P = 40;
    float P2M=1/M2P;

    Player *player;
    //vector<Bot*> bots;
    vector<AI*> Ai;

    QTimer *timer;

    b2World* world;

    void initializeGL();

    void resizeGL(int nWidth, int nHeight);

    void paintGL();

    void mousePressEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    b2Body* addRect(float x, float y, float width, float height, bool dyn, Textures::Type type);

    b2Body* addSpecRect ();

    void drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, TextureData *textureData);
    void drawPolygon(b2Vec2* points, int count, b2Vec2 center,float angle, KeyLineData *keyLineData);
    void drawCircle(float radius, b2Vec2 center, KeyLineData *keyLineData);
    void drawChain(b2Vec2* points, b2Vec2 center, int count, KeyLineData *keyLineData);

    void addPlayer ();
    void createWorld();

    b2Body *addBot(Bot* bot);

    Textures textures;
};

#endif // GAMEWIDGET_H
