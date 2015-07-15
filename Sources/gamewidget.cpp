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

#include "gamewidget.h"
#include <QDebug>

/**
 * @brief GameWidget::GameWidget - contructor
 * @param parent - parent widget instance
 */
GameWidget::GameWidget(QWidget *parent) : QGLWidget(parent) {
}

void GameWidget::initializeGL() {
    glMatrixMode(GL_PROJECTION);
                    glOrtho(0,WIDTH,HEIGHT,0,-1,1);
            glMatrixMode(GL_MODELVIEW);
            glClearColor(0,0,0,1);
            world=new b2World(b2Vec2(0.0,9.81));
            addRect(WIDTH/2,HEIGHT-50,1,1,false);
}

void GameWidget::resizeGL(int nWidth, int nHeight) {
    glViewport(0, 0, nHeight, nHeight); // установка точки обзора
    glMatrixMode(GL_PROJECTION); // установка режима матрицы
    glLoadIdentity(); // загрузка матрицы
}

void GameWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
            glLoadIdentity();
            b2Body* tmp=world->GetBodyList();
            b2Vec2 points[4];
            while(tmp)
            {
                    for(int i=0;i<4;i++)
                            points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);

                    drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());
                    tmp=tmp->GetNext();
            }
            world->Step(1.0/30.0,8,3);
}

void GameWidget::mousePressEvent(QMouseEvent *event) {

    qDebug()<<"Mouse";
    addRect(event->pos().x(), event->pos().x(), 20, 20, true);
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    paintGL();
}

b2Body* GameWidget::addRect(int x, int y, int w, int h, bool dyn) {
    b2BodyDef bodydef;
    bodydef.position.Set(x*P2M,y*P2M);
    if(dyn)
            bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);

    b2PolygonShape shape;
    shape.SetAsBox(P2M*w/2,P2M*h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    body->CreateFixture(&fixturedef);
}

void GameWidget::drawSquare(b2Vec2* points,b2Vec2 center,float angle) {
    glColor3f(1,1,1);
            glPushMatrix();
                    //glTranslatef(center.x,center.y,0);
                    glRotatef(angle*180.0/M_PI,0,0,1);
                    glBegin(GL_QUADS);
                      glVertex2f(0.5, 0.5);
                      glVertex2f(-0.5, 0.5);
                      glVertex2f(-0.5, -0.5);
                      glVertex2f(0.5, -0.5);
                    glEnd();
            glPopMatrix();
}
