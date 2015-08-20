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
#include "GameObjects/userdata.h"
#define M_PI		3.14159265358979323846


/**
 * @brief GameWidget::GameWidget - contructor
 * @param parent - parent widget instance
 */

GameWidget::GameWidget(QWidget *parent) : QGLWidget(parent) {
    timer = new QTimer;
    connect (timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);
    world=new b2World(b2Vec2(0.0,-9.81));
    player = new Player();
    addPlayer();
    // world->SetContactListener(player->contactListener);
    addRect(0,-HEIGHT,WIDTH*2,50,false);
    addRect(0,HEIGHT,WIDTH*2,50,false);
    addRect(-WIDTH,0,50,HEIGHT*2,false);
    addRect(WIDTH,0,50,HEIGHT*2,false);
    addRect(0,0,40,40,false);
    addSpecRect();
}

void GameWidget::addPlayer (){
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    player->setBody(body);
    body->SetUserData((void*) new UserData (Textures::Type::PLAYER));
    b2PolygonShape shape;
    shape.SetAsBox(1,2);

    b2FixtureDef fixturedef;
    // fixturedef.friction = 5;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);
    mainFixture->SetUserData( (void*)1 );
    //mainFixture->SetUserData((void*) new UserData (Textures::Type::TEST1));

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0,-2), 0);
    fixturedef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);
    //footSensorFixture->SetUserData((void*) new UserData (Textures::Type::TEST2));

    footSensorFixture->SetUserData( (void*)3 );
}

void GameWidget::updateGame(){
    // updateGL();
    // paintGL();
    updatePlayerJump();
    player->applyForce();
}

void GameWidget::updatePlayerJump(){
    if (player->body->GetLinearVelocity().y==0) player->allowJump();
}

void GameWidget::initializeGL() {
    resize(WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0,WIDTH,HEIGHT,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0,0,0,1);

    textures.loadAll();
}

void GameWidget::resizeGL(int nWidth, int nHeight) {
    WIDTH = nWidth;
    HEIGHT = nHeight;
    glViewport(0, 0, nWidth, nHeight); // установка точки обзора
    glMatrixMode(GL_PROJECTION); // установка режима матрицы
    glLoadIdentity(); // загрузка матрицы
}

void GameWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //background

    UserData* data1 = new UserData (Textures::Type::TEST2);
    b2Vec2 points1[4] = {b2Vec2(-100,-100),b2Vec2(100,-100),b2Vec2(100,100),b2Vec2(-1,100)};
    drawSquare (points1,b2Vec2(0,0),0, data1);


    b2Body* tmp=world->GetBodyList();
    b2Vec2 points[4];
    while(tmp)
    {


        for(int i=0;i<4;i++){
            points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
        }
        Color bodyColor;
        switch (tmp->GetType())
        {
        case b2_staticBody: bodyColor.setColor (80, 80, 80); break;
        case b2_dynamicBody: bodyColor.setColor (50, 50, 170); break;
        default: break;
        }
        UserData* data = static_cast<UserData*>(tmp->GetUserData());
        drawSquare (points, tmp->GetWorldCenter(), tmp->GetAngle(), data);
        tmp=tmp->GetNext();
    }

    world->Step(1.0/30.0,8,3);
    updateGame();
}

void GameWidget::mousePressEvent(QMouseEvent *event) {
    addRect((event->pos().x()-WIDTH/2)*2, -(event->pos().y()-HEIGHT/2)*2, 80, 80, true);
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Left || key == Qt::Key_A)
        player->moveState = Player::MS_LEFT;
    if (key == Qt::Key_Right || key == Qt::Key_D)
        player->moveState = Player::MS_RIGHT;
    if (key == Qt::Key_Up || key == Qt::Key_W)
        player->jump();
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    int key = event->key();
    if ( (key == Qt::Key_Left || key == Qt::Key_A) && player->moveState == Player::MS_LEFT
         ||(key == Qt::Key_Right || key == Qt::Key_D) && player->moveState == Player::MS_RIGHT )
        player->moveState = Player::MS_STAND;
}

b2Body* GameWidget::addRect(int x, int y, int w, int h, bool dyn) {
    b2BodyDef bodydef;
    bodydef.position.Set(x*P2M,y*P2M);
    if(dyn)
        bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    body->SetAngularVelocity(0.5);
    b2PolygonShape shape;
    shape.SetAsBox(P2M*w/2,P2M*h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    fixturedef.filter.groupIndex=1;

    body->CreateFixture(&fixturedef);

    body->SetUserData((void*) new UserData (Textures::Type::CRATE));
    return body;
}

b2Body* GameWidget::addSpecRect() {
    b2BodyDef bodydef;
    bodydef.position.Set(0,0);

    bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    body->SetAngularVelocity(-5);
    b2PolygonShape shape;
    shape.SetAsBox(4,4);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    fixturedef.filter.groupIndex=1;

    body->CreateFixture(&fixturedef);

    body->SetUserData((void*) new UserData (Textures::Type::TEST3));
    return body;
}

void GameWidget::drawSquare(b2Vec2* points, b2Vec2 center, float angle, UserData* userData) {
    struct point {float x; float y;};
    point  squarePoints [4] = {{0.0f, 0.0f},{1.0f, 0.0f},{1.0f, 1.0f},{0.0f, 1.0}};
    glPushMatrix();
    glColor3f(1, 1 ,1);
    glTranslatef(center.x*M2P/WIDTH,center.y*M2P/WIDTH,0);
     glRotatef(angle*180.0/M_PI,0,0,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures.getTextureId(userData->textureType));//TOCHANGE?
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2f(squarePoints[i].x, squarePoints[i].y);
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
