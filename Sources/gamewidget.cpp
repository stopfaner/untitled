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

}


void GameWidget::createWorld(){
    timer = new QTimer;
    connect (timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);
    world=new b2World(b2Vec2(0.0,-9.81));
    player = new Player();
    addPlayer();
    // world->SetContactListener(player->contactListener);
    addRect(0,-HEIGHT*P2M,1000,3,false,Textures::Type::CRATE);
    addRect(0,HEIGHT*P2M,1000,3,false,Textures::Type::CRATE);
    //addRect(-WIDTH*P2M,0,3,10000,false,Textures::Type::BOT);
    //addRect(WIDTH*P2M,0,3,10000,false,Textures::Type::PLAYER);
    //addRect(100,100,100,100,false,Textures::Type::CRATE);
    //addRect(0,-HEIGHT,WIDTH*8,50,false);
    //addRect(0,HEIGHT,WIDTH*8,50,false);
    // addRect(-WIDTH,0,50,HEIGHT*2,false);
    // addRect(WIDTH,0,50,HEIGHT*2,false);
    //addRect(0,0,40,40,false);
    addSpecRect();

    Bot *bot;
    bot = new Bot();
    bot->setBody(addBot(bot));
    AI *ai;
    ai = new AI(player,bot);
    Ai.push_back(ai);
}

void GameWidget::addPlayer (){
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    player->setBody(body);
    player->userData = new UserData (textures.getTexture(Textures::Type::RUN));
    player->userData->isPlayer = true;
    body->SetUserData((void*) player->userData);
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

b2Body *GameWidget::addBot(Bot* bot) {
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    bot->userData = new UserData (textures.getTexture(Textures::Type::BOT));
    body->SetUserData((void*) bot->userData);
    b2PolygonShape shape;
    shape.SetAsBox(1,2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);
    mainFixture->SetUserData( (void*)1 );

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0,-2), 0);
    fixturedef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);

    footSensorFixture->SetUserData( (void*)3 );
    return body;
}

void GameWidget::updateGame(){
    // updateGL();
    // paintGL();
    player->updatePlayerJump();
    player->applyForce();
    for(unsigned int i = 0; i < Ai.size(); i++) {
        AI *temp = Ai.at(i);
        temp->updateAI();
        temp->bot->applyForce();
        temp->bot->updateBotJump();

        if (temp->bot->moveState == Player::MoveState::MS_LEFT)
            temp->bot->userData->isMirrored = true;
        if (temp->bot->moveState == Player::MoveState::MS_RIGHT)
            temp->bot->userData->isMirrored = false;
    }

    //setting animation type

    if (player->moveState == Player::MoveState::MS_LEFT)
        player->userData->isMirrored = true;
    if (player->moveState == Player::MoveState::MS_RIGHT)
        player->userData->isMirrored = false;
    if (player->moveState == Player::MoveState::MS_LEFT
            || player->moveState == Player::MoveState::MS_RIGHT){
        if (player->userData->texture_p->type != Textures::Type::RUN)
        player->userData->setTexture(textures.getTexture(Textures::Type::RUN));
    }
    else
        if (player->userData->texture_p->type != Textures::Type::PLAYER)
            player->userData->setTexture(textures.getTexture(Textures::Type::PLAYER));
}

void GameWidget::initializeGL() {
    resize(WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0,WIDTH,HEIGHT,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0,0,0,1);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    textures.loadAll();
    createWorld();
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
    //glViewport(-player->body->GetWorldCenter().x*M2P/2,
    //           -player->body->GetWorldCenter().y*M2P/2, WIDTH, HEIGHT);

    glLoadIdentity();

    //background

    UserData* data1 = new UserData (textures.getTexture(Textures::Type::BACKGROUND));
    b2Vec2 pointsBackground[4] = {b2Vec2(-WIDTH*P2M,-WIDTH*P2M),b2Vec2(WIDTH*P2M,-WIDTH*P2M),
                                  b2Vec2(WIDTH*P2M,WIDTH*P2M),b2Vec2(-WIDTH*P2M,WIDTH*P2M)};
    drawSquare (pointsBackground,b2Vec2(0,0),0, data1);

    //transparent square

    b2Vec2 points1[4] = {b2Vec2(-5,-5),b2Vec2(5,-5),
                         b2Vec2(5,5),b2Vec2(-5,5)};
    drawSquare (points1,b2Vec2(0,0),0, Color(255, 255, 0, 100));

    //bodies

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
        data->changeFrame();
        tmp=tmp->GetNext();
    }

    //update Box2D

    world->Step(1.0/30.0,8,3);
    updateGame();
}

void GameWidget::mousePressEvent(QMouseEvent *event) {
    //addRect((event->pos().x()-WIDTH/2)*2, -(event->pos().y()-HEIGHT/2)*2, 80, 80, true, Textures::Type::CRATE);
    addRect((event->pos().x()+player->body->GetWorldCenter().x*M2P/2-WIDTH/2)*2*P2M,
            -(event->pos().y()-player->body->GetWorldCenter().y*M2P/2-HEIGHT/2)*2*P2M, 2, 2, true, Textures::Type::CRATE);
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Left || key == Qt::Key_A)
        player->moveState = Player::MS_LEFT;
    if (key == Qt::Key_Right || key == Qt::Key_D)
        player->moveState = Player::MS_RIGHT;
    if (key == Qt::Key_Up || key == Qt::Key_W)
        player->jump();
    if (key == Qt::Key_Escape) this->close();
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    int key = event->key();
    if ( (key == Qt::Key_Left || key == Qt::Key_A) && player->moveState == Player::MS_LEFT
         ||(key == Qt::Key_Right || key == Qt::Key_D) && player->moveState == Player::MS_RIGHT )
        player->moveState = Player::MS_STAND;
}

b2Body* GameWidget::addRect(float x, float y, float w, float h, bool dyn, Textures::Type type) {
    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    if(dyn)
        bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    body->SetAngularVelocity(0.5);
    b2PolygonShape shape;
    shape.SetAsBox(w/2,h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=3.0;
    fixturedef.filter.groupIndex=1;

    body->CreateFixture(&fixturedef);

    body->SetUserData((void*) new UserData (textures.getTexture(type)));
    return body;
}

b2Body* GameWidget::addSpecRect() {
    b2BodyDef bodydef;
    bodydef.position.Set(0,0);

    bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    body->SetAngularVelocity(-5);
    b2PolygonShape shape;
    shape.SetAsBox(3,3);

    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    fixturedef.filter.groupIndex=1;

    body->CreateFixture(&fixturedef);

    body->SetUserData((void*) new UserData (textures.getTexture(Textures::Type::CRATE)));
    return body;
}

void GameWidget::drawSquare(b2Vec2* points, b2Vec2 center, float angle, UserData* userData) {

    //setting texture's points

    struct point {float x; float y;};
    point  texPoints [4];
    point  straightImage [4] = {{0.0f, 0.0f},{1.0f, 0.0f},{1.0f, 1.0f},{0.0f, 1.0f}};
    point  mirroredImage [4] = {{1.0f, 0.0f},{0.0f, 0.0f},{0.0f, 1.0f},{1.0f, 1.0f}};
    if (userData->isMirrored)
        for (int i = 0; i<4; i++)
            texPoints[i] = mirroredImage[i];
    else
        for (int i = 0; i<4; i++)
            texPoints[i] = straightImage[i];

    //loading sprite tile

    int tileColumn = userData->currentFrameN % userData->texture_p->columns;
    int tileRow = userData->currentFrameN / userData->texture_p->columns;
    for (int i = 0; i < 4; ++i){

        texPoints[i].x /= userData->texture_p->columns;
        texPoints[i].y /= userData->texture_p->rows;
        texPoints[i].x += (float) tileColumn/userData->texture_p->columns;
        texPoints[i].y += (float) tileRow/userData->texture_p->rows;
    }

    glPushMatrix();
    glColor3f(1, 1 ,1);
    if(!userData->isPlayer)
        glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);

    glRotatef(angle*180.0/M_PI,0,0,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, userData->texture_p->id);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++){
        glTexCoord2f(texPoints[i].x, texPoints[i].y);
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glColor3f(1, 1 ,1);
}

void GameWidget::drawSquare(b2Vec2* points, b2Vec2 center,float angle, Color color) {

    glColor4f(color.red, color.green, color.blue, color.alpha);
    glPushMatrix();
    glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/2,center.y*M2P/WIDTH+player->body->GetWorldCenter().y*M2P/2,0);
    glRotatef(angle*180.0/M_PI,0,0,1);
    glBegin(GL_QUADS);
    for(int i=0;i<4;i++)
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    glEnd();
    glPopMatrix();
}
