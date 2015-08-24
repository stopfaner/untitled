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
    world = new b2World(b2Vec2(0.0,-9.81));
    b2AABB *borderWorld = new b2AABB();
    borderWorld->lowerBound.Set(-1000.0, -1000.0);
    borderWorld->upperBound.Set(1000.0, 1000.0);
    player = new Player(textures.getTexture(Textures::Type::RUN));
    addPlayer();
    addRect(0,-1000,1000,2,false,Textures::Type::WALL);
    addRect(0,1000,1000,2,false,Textures::Type::WALL);
    addRect(-1000,0,2,1000,false,Textures::Type::WALL);
    addRect(1000,0,2,10000,false,Textures::Type::WALL);
    // addRect(0,-260,500,500,false,Textures::Type::WALL);

    //creatin chain

    srand( time(0) );

    b2BodyDef chaindef;
    chaindef.position.Set(0, 0);
    chaindef.type = b2_staticBody;
    b2Body* chain = world->CreateBody(&chaindef);

    b2Vec2 vs[300];
    float x = -1000;
    int i = 0;
    while (x < 1000){
        float dx = rand()%5 + 5;
        float dy = rand()%4 - 2;
        x += dx;
        vs[i].Set(x, dy - 15.0f);
        ++i;

    }

    b2ChainShape chainShape;

    void* chainData =(void*) new UserData();
    chain->SetUserData(chainData);

    chainShape.CreateChain(vs, i);

    b2FixtureDef chainFixtureDef;
    chainFixtureDef.shape = &chainShape;
    chainFixtureDef.density = 1.0;

    chain->CreateFixture(&chainFixtureDef);

    chain->SetUserData(chainData);


    Room room(&textures,world);
    // room.CreateRoom(b2Vec2(-15, 1), b2Vec2(20, 20), 0.5, 13, 10);
    room.CreateRoom(b2Vec2(0, 0), b2Vec2(40, 20), 1, 6, 15);
    room.CreateRoom(b2Vec2(39, 0), b2Vec2(40, 20), 1, 15, 5);
    addSpecRect();

    Bot *bot;
    bot = new Bot(textures.getTexture(Textures::Type::BOT));
    bot->setBody(addBot(bot));
    AI *ai;
    ai = new AI(player,bot);
    Ai.push_back(ai);

    //car

    b2Body* mainPlank = addRect(0, 0, 5, 0.5, true, Textures::Type::CRATE);
    b2Body* littlePlank1 = addRect(2.5, 0, 0.5, 3, true, Textures::Type::CRATE);
    b2Body* littlePlank2 = addRect(-2.5, 0, 0.5, 3, true, Textures::Type::CRATE);
    b2RevoluteJointDef jointDef;

    jointDef.maxMotorTorque = 1500.0f;
    jointDef.motorSpeed = 5.0f;
    jointDef.enableMotor = true;

    jointDef.Initialize(mainPlank, littlePlank1, b2Vec2(2.5, 0));
    world->CreateJoint( &jointDef );
    jointDef.Initialize(mainPlank, littlePlank2, b2Vec2(-2.5, 0));
    world->CreateJoint( &jointDef );


    b2Body* mainPlank2 = addRect(0, 5, 5, 0.5, true, Textures::Type::CRATE);
    b2Body* rule = addRect(0, 6.5, 0.5, 3, true, Textures::Type::CRATE);
    //circle
    b2BodyDef bodydef;
    bodydef.position.Set(2.5, 5);
    bodydef.type = b2_dynamicBody;
    b2Body* circle = world->CreateBody(&bodydef);

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0);
    circleShape.m_radius = 1;

    b2FixtureDef fixturedef;
    fixturedef.shape = &circleShape;
    fixturedef.density = 1.0;

    circle->CreateFixture(&fixturedef);
    //
    bodydef.position.Set(-2.5, 5);
    b2Body* circle2 = world->CreateBody(&bodydef);
    circle2->CreateFixture(&fixturedef);

    jointDef.Initialize(mainPlank2, circle, b2Vec2(2.5, 5));
    world->CreateJoint( &jointDef );
    jointDef.Initialize(mainPlank2, circle2, b2Vec2(-2.5, 5));
    world->CreateJoint( &jointDef );
    b2WeldJointDef weldJointDef;
    weldJointDef.Initialize(rule, mainPlank2, b2Vec2(0, 5));
    world->CreateJoint( &weldJointDef );


    //creating ladder

    b2BodyDef bodydefLadder;
    bodydefLadder.position.Set(-25, -10);
    bodydefLadder.type=b2_staticBody;
    b2Body* body=world->CreateBody(&bodydefLadder);

    b2PolygonShape shape;
    shape.SetAsBox(1, 10);

    b2FixtureDef fixturedefLadder;
    fixturedefLadder.shape = &shape;
    fixturedefLadder.isSensor = true;
    fixturedefLadder.density = 3.0;
    fixturedefLadder.filter.groupIndex = 1;

    b2Fixture* ladderFixture = body->CreateFixture(&fixturedefLadder);
    void* ladderData = new Ladder (textures.getTexture(Textures::Type::LADDER));
    body->SetUserData(ladderData);
    ladderFixture->SetUserData(ladderData);


}

void GameWidget::addPlayer (){
    float playerWidth = 2; float playerHeight = 4;
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    player->setBody(body);
    player->isPlayer = true;
    body->SetUserData((void*) player);
    b2PolygonShape shape;
    shape.SetAsBox(playerWidth/2, playerHeight/2);

    b2FixtureDef fixturedef;
    // fixturedef.friction = 5;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);
    mainFixture->SetUserData((void*) new BodyPart (BodyPart::Type::MainFixture));

    b2PolygonShape sensorShape;
    sensorShape.SetAsBox(playerWidth/2*0.7, 0.1, b2Vec2(0,-2), 0);
    fixturedef.shape = &sensorShape;
    fixturedef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);
    footSensorFixture->SetUserData((void*) new BodyPart (BodyPart::Type::FootSensor));

    //joint example
    /*
    b2Body* body1 = addRect(2, 0, 4, 0.6, true, Textures::Type::CRATE);
    b2WeldJointDef jointDef;

    jointDef.Initialize(player->body, body1,
                        b2Vec2(player->body->GetWorldCenter().x + playerWidth/2,
                               player->body->GetWorldCenter().y));

    b2WeldJointDef* joint = (b2WeldJointDef*)world->CreateJoint( &jointDef );
*/

}

b2Body *GameWidget::addBot(Bot* bot) {
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    body->SetUserData((void*) bot);
    b2PolygonShape shape;
    shape.SetAsBox(1,2);


    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);

    mainFixture->SetUserData((void*) new BodyPart (BodyPart::Type::MainFixture));

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0,-2), 0);
    fixturedef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);

    footSensorFixture->SetUserData((void*) new BodyPart (BodyPart::Type::FootSensor));
    return body;
}

void GameWidget::updateGame(){
    player->update(&textures);
    if (player->jumpCooldown) --player->jumpCooldown;

    for(unsigned int i = 0; i < Ai.size(); i++) {
        AI *temp = Ai.at(i);
        temp->updateAI();
        temp->bot->applyForce();
        temp->bot->updateBotJump();

        if (temp->bot->moveState == Player::MoveState::MS_LEFT)
            temp->bot->isMirrored = true;
        if (temp->bot->moveState == Player::MoveState::MS_RIGHT)
            temp->bot->isMirrored = false;
    }

    //setting animation type

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
    drawPolygon (pointsBackground, 4, b2Vec2(0, 0), 0, data1);

    //bodies

    b2Body* tmp=world->GetBodyList();

    while(tmp)
    {
        b2Fixture* curFixture = tmp->GetFixtureList();
        while(curFixture){
            b2Shape::Type curShapeType = curFixture->GetShape()->GetType();
            if (curShapeType ==  b2Shape::e_polygon){
                int count = ((b2PolygonShape*)curFixture->GetShape())->GetVertexCount();
                b2Vec2 points[count];
                for(int i=0; i < count; i++){
                    points[i]=((b2PolygonShape*)curFixture->GetShape())->GetVertex(i);
                }

                UserData* data = static_cast<UserData*>(tmp->GetUserData());
                if (data->hasTexture){
                    drawPolygon (points, count, tmp->GetWorldCenter(), tmp->GetAngle(), data);
                    data->changeFrame();
                }
                else drawPolygon (points, count, tmp->GetPosition(), tmp->GetAngle(), Color());
            }
            else
                if (curShapeType == b2Shape::e_circle)
                    drawCircle(((b2CircleShape*)curFixture->GetShape())->m_radius, tmp->GetWorldCenter(), Color(80, 120, 90, 150));
                else
                    if (curShapeType == b2Shape::e_chain){
                        int edgeCount = ((b2ChainShape*)curFixture->GetShape())->GetChildCount();
                        //one element for start point
                        b2Vec2 points [edgeCount+1];


                        b2EdgeShape firstEdge;
                        ((b2ChainShape*)curFixture->GetShape())->GetChildEdge(&firstEdge, 0);
                        points[0] = firstEdge.m_vertex1;
                        points[1] = firstEdge.m_vertex2;


                        for (int i = 1; i < edgeCount; ++i){
                            b2EdgeShape edge;
                            ((b2ChainShape*)curFixture->GetShape())->GetChildEdge(&edge, i);
                            points[i+1] = edge.m_vertex2;

                        }

                        drawChain (points, tmp->GetWorldCenter(), edgeCount + 1, Color(255, 100, 0, 255));

                    }
            curFixture=curFixture->GetNext();
        }
        tmp=tmp->GetNext();

    }

    //meter rectangle

    b2Vec2 points1[4] = {b2Vec2(-1, -2),b2Vec2(0, -2),
                         b2Vec2(0, -2.5),b2Vec2(-1, -2.5)};
    drawPolygon (points1, 4, b2Vec2(0, 0), 0, Color(255, 255, 0, 100));

    //update Box2D

    world->Step(1.0/30.0, 8, 3);
    updateGame();
}

void GameWidget::mousePressEvent(QMouseEvent *event) {
    Qt::MouseButtons mouseButtons = event->buttons();
    if (mouseButtons == Qt::LeftButton)
        addRect((event->pos().x()+player->body->GetWorldCenter().x*M2P/2-WIDTH/2)*2*P2M,
                -(event->pos().y()-player->body->GetWorldCenter().y*M2P/2-HEIGHT/2)*2*P2M, 2, 2, true, Textures::Type::CRATE);
    else
        if (mouseButtons == Qt::RightButton){
            b2Body* body1 = addRect(0, 6, 10, 2, true, Textures::Type::CRATE);
            b2Body* body2 = addRect(0, 6, 2, 10, true, Textures::Type::CRATE);
            b2WeldJointDef jointDef;
            jointDef.Initialize(body1, body2, b2Vec2(0, 6));
            b2WeldJointDef* joint = (b2WeldJointDef*)world->CreateJoint( &jointDef );
        }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Left || key == Qt::Key_A)
        player->moveState = Player::MS_LEFT;
    if (key == Qt::Key_Right || key == Qt::Key_D)
        player->moveState = Player::MS_RIGHT;
    if (key == Qt::Key_Up || key == Qt::Key_W)
        player->moveStateVertical = Player::MSV_UP;
    if (key == Qt::Key_Down || key == Qt::Key_S)
        player->moveStateVertical = Player::MSV_DOWN;
    if (key == Qt::Key_E)
        player->useObject();
    if (key == Qt::Key_Escape) this->close();
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    int key = event->key();
    if ( (key == Qt::Key_Left || key == Qt::Key_A) && player->moveState == Player::MS_LEFT
         ||(key == Qt::Key_Right || key == Qt::Key_D) && player->moveState == Player::MS_RIGHT )
        player->moveState = Player::MS_STAND;
    if ( (key == Qt::Key_Up || key == Qt::Key_W) && player->moveStateVertical == Player::MSV_UP
            ||(key == Qt::Key_Down || key == Qt::Key_S) && player->moveStateVertical == Player::MSV_DOWN )
        player->moveStateVertical = Player::MSV_STAND;
}



b2Body* GameWidget::addRect(float x, float y, float w, float h, bool dyn, Textures::Type type) {
    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    if(dyn)
        bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    body->SetAngularVelocity(0.5);
    b2PolygonShape shape;
    shape.SetAsBox(w/2, h/2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 3.0;
    fixturedef.filter.groupIndex = 1;

    body->CreateFixture(&fixturedef);

    body->SetUserData((void*) new UserData (textures.getTexture(type)));
    return body;
}

b2Body* GameWidget::addSpecRect() {
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);

    bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);

    b2PolygonShape shape;

    b2Vec2 vertices [5];
    float sizeK = 0.3;
    vertices[0].Set(-1, -1);
    vertices[1].Set(-1, 1);
    vertices[2].Set(1, 1);
    vertices[3].Set(2, 0);
    vertices[4].Set(1, -1);
    for (int i = 0; i < 5; i++){
        vertices[i].x *= sizeK;
        vertices[i].y *= sizeK;
    }
    shape.Set(vertices, 5);


    b2FixtureDef fixturedef;
    fixturedef.shape=&shape;
    fixturedef.density=1.0;
    fixturedef.filter.groupIndex=1;

    body->CreateFixture(&fixturedef);

    body->ApplyLinearImpulse(b2Vec2(-body->GetMass() * 100, 0), b2Vec2(0, 0), true);

    body->SetUserData((void*) new UserData ());
    return body;
}

void GameWidget::drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, UserData* userData) {

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

    //choosing sprite tile

    int tileColumn = userData->currentFrameN % userData->texture_p->columns;
    int tileRow = userData->currentFrameN / userData->texture_p->columns;
    for (int i = 0; i < 4; ++i){

        texPoints[i].x /= userData->texture_p->columns;
        texPoints[i].y /= userData->texture_p->rows;
        texPoints[i].x += (float) tileColumn/userData->texture_p->columns;
        texPoints[i].y += (float) tileRow/userData->texture_p->rows;
    }

    //drawing texture

    glPushMatrix();
    glColor3f(1, 1, 1);

    if(!userData->isPlayer)
        glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);

    glRotatef(angle*180.0/M_PI,0,0,1);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, userData->texture_p->id);

    glBegin(GL_POLYGON);
    for(int i = 0; i < count; i++){
        glTexCoord2f(texPoints[i].x, texPoints[i].y);
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void GameWidget::drawChain(b2Vec2* points, b2Vec2 center, int count, Color color) {

    glColor4f(color.red, color.green, color.blue, color.alpha);
    glPushMatrix();
    glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);

    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < count; i++)
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, Color color) {

    glColor4f(color.red, color.green, color.blue, color.alpha);
    glPushMatrix();
    glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);
    //glTranslatef (center.x*M2P/WIDTH, center.y*M2P/WIDTH, 0);
    glRotatef(angle*180.0/M_PI, 0, 0, 1);
    glBegin(GL_POLYGON);
    for(int i=0;i<count;i++)
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawCircle(float radius, b2Vec2 center, Color color){

    glColor4f(color.red, color.green, color.blue, color.alpha);
    glPushMatrix();

    glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);

    glBegin(GL_LINE_LOOP);

    for (int i=0; i < 360; i++)
    {
        float rad = i*M_PI/180.0f;
        glVertex2f(cos(rad)*radius*M2P/WIDTH, sin(rad)*radius*M2P/WIDTH);
    }

    glEnd();
    glPopMatrix();
}
