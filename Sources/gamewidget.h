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
#include "GameObjects/Rooms/room.h"
#include "GameObjects/userdata.h"


/**
 * @brief The GameWidget class describes main window activity
 */
using namespace std;

class Color
{
public:
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
    Color ()
    {
        red = 1;
        green = 1;
        blue = 1;
        alpha = 1;
    }
    Color (short red, short green, short blue, short alpha = 255)
    {
        setColor (red, green, blue, alpha);
    }
    setColor (short red, short green, short blue, short alpha = 255)
    {
        this->red = (float) red / 255;
        this->green = (float) green / 255;
        this-> blue = (float) blue / 255;
        this-> alpha = (float) alpha / 255;
    }
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

    void drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, UserData *userData);
    void drawPolygon(b2Vec2* points, int count, b2Vec2 center,float angle, Color color);
    void drawCircle(float radius, b2Vec2 center, Color color);
    void drawChain(b2Vec2* points, b2Vec2 center, int count, Color color);

    void addPlayer ();
    void createWorld();

    b2Body *addBot(Bot* bot);

    GLuint tex;
    Textures textures;
};

#endif // GAMEWIDGET_H
