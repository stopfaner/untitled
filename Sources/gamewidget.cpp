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
#define M_PI 3.14

/**
 * @brief GameWidget::GameWidget - contructor
 * @param parent - parent widget instance
 */
GameWidget::GameWidget(QWidget *parent) : QGLWidget(parent) {
    timer = new QTimer;
    connect (timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start();
}

void GameWidget::initializeGL() {
    glMatrixMode(GL_PROJECTION);
    glOrtho(0,WIDTH,HEIGHT,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0,0,0,1);
    world=new b2World(b2Vec2(0.0,-9.81));
   // addRect(0,0,WIDTH,20,false);
    addRect(0,0,300,20,false);
}

void GameWidget::resizeGL(int nWidth, int nHeight) {
    glViewport(0, 0, nWidth, nHeight); // установка точки обзора
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
        for(int i=0;i<4;i++){
            points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
        }
        drawSquare(points,tmp->GetWorldCenter(),tmp->GetAngle());
        tmp=tmp->GetNext();
    }
       world->Step(1.0/30.0,8,3);

}

void GameWidget::mousePressEvent(QMouseEvent *event) {

    qDebug()<<"Mouse"<<endl<<event->pos().x()<<" "<<event->pos().y()<<endl;
float toChange = 2;
addRect((event->pos().x()-WIDTH/2)*toChange, -(event->pos().y()-HEIGHT/2)*toChange, 20, 20, true);
    //addRect(event->pos().x()-WIDTH/2, -(event->pos().y()-HEIGHT/2), 20, 20, true);
    updateGL();
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    paintGL();
}

b2Body* GameWidget::addRect(int x, int y, int w, int h, bool dyn) {
    qDebug()<<"Adding rectangle";
    qDebug()<<x<<" "<<y;
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
    qDebug()<<"Drawing square";
    qDebug()<<"coords in meters "<<center.x<<" "<<center.y;
    glColor3f(1,1,1);
    glPushMatrix();
    qDebug()<<"drawing coords "<<center.x*M2P/WIDTH<<" "<<center.y*M2P/HEIGHT;
    glTranslatef(center.x*M2P/WIDTH,center.y*M2P/HEIGHT,0);
    glRotatef(angle*180.0/M_PI,0,0,1);
    glBegin(GL_QUADS);
float tempKoefx = 100./WIDTH;//100 meters in screen
float tempKoefy = 100./HEIGHT;
    for(int i=0;i<4;i++)
         glVertex2f(points[i].x*tempKoefx,points[i].y*tempKoefy);
       // glVertex2f(points[i].x*M2P,points[i].y*M2P);
    glEnd();
    glPopMatrix();
    /*  glColor3f(1,1,1);
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


    qglColor(Qt::white);
    glPushMatrix();
    glTranslatef(center.x*M2P,center.y*M2P,0);
    glRotatef(angle*180.0/M_PI,0,0,1);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glVertex2f(points[i].x*M2P,points[i].y*M2P);
    }
    glEnd();
    glPopMatrix();*/
}
