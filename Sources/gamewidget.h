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

/**
 * @brief The GameWidget class describes main window activity
 */
class GameWidget : public QGLWidget {

public:
    /**
     * @brief GameWidget - constructor
     * @param parent - parent widget instance
     */
    GameWidget(QWidget *parent = 0);

private:
    int WIDTH = 1900;
    int HEIGHT = 1000;

    float M2P = 20;
    float P2M=1/M2P;

    b2World* world;

    void initializeGL();

    void resizeGL(int nWidth, int nHeight);

    void paintGL();

    void mousePressEvent(QMouseEvent* event);

    void keyPressEvent(QKeyEvent* event);

    b2Body* addRect(int x, int y, int width, int height, bool dyn = true);

    void drawSquare(b2Vec2* points, b2Vec2 center, float angle);

};

#endif // GAMEWIDGET_H
