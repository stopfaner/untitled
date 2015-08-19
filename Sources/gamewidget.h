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
#include <Box2D/Box2D.h>
#include <QTimer>
#include "GameObjects/Characters/player.h"

/**
 * @brief The GameWidget class describes main window activity
 */

//TOMOVE
class Color
{
public:
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    Color ()
    {
        red = 1;
        green = 1;
        blue = 1;
    }
    Color (short red, short green, short blue)
    {
        setColor (red, green, blue);

    }
    setColor (short red, short green, short blue)
    {
        this->red = (float) red / 255;
        this->green = (float) green / 255;
        this-> blue = (float) blue / 255;
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
    int WIDTH = 480;
    int HEIGHT = 480;

    float M2P = 20;
    float P2M=1/M2P;

    Player *player;
    QTimer *timer;

    b2World* world;

    void initializeGL();

    void resizeGL(int nWidth, int nHeight);

    void paintGL();

    void mousePressEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    b2Body* addRect(int x, int y, int width, int height, bool dyn = true);

    b2Body* addSpecRect ();

    void drawSquare(b2Vec2* points, b2Vec2 center, float angle, Color color);

    void updatePlayerJump ();

};

#endif // GAMEWIDGET_H
