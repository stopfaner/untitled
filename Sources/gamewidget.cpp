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



    addPlayer();
    addRect(0,-1000,1000,2,false,Textures::Type::WALL);
    addRect(0,1000,1000,2,false,Textures::Type::WALL);
    addRect(-1000,0,2,1000,false,Textures::Type::WALL);
    addRect(1000,0,2,10000,false,Textures::Type::WALL);
    // addRect(0,-260,500,500,false,Textures::Type::WALL);

    //creating chain

    srand( time(0) );

    b2BodyDef chaindef;
    chaindef.position.Set(0, 0);
    chaindef.type = b2_staticBody;
    b2Body* chain = world->CreateBody(&chaindef);

    b2Vec2 vs[1000];
    float x = -1000;
    int i = 0;
    while (x < 1000){
        float dx = rand()%5 + 5;
        float dy = rand()%4 - 2;
        x += dx;
        if (i>0){
            vs[i].Set(x - dx / 2, ((vs[i-1].y + (dy-15.0f)) /2));
            ++i;
        }
        vs[i].Set(x, dy - 15.0f);
        ++i;
    }
    vs[0].x = -1000;
    vs[i].x = 1000;


    b2ChainShape chainShape;

    DisplayData* chainDD = (DisplayData*) new KeyLineData (Color(255, 0, 0));
    UserData* chainUD = new UserData(chainDD);
    chain->SetUserData((void*)chainUD);

    chainShape.CreateChain(vs, i);

    b2FixtureDef chainFixtureDef;
    chainFixtureDef.shape = &chainShape;
    chainFixtureDef.density = 1.0;

    b2Fixture* chainFix = chain->CreateFixture(&chainFixtureDef);

    chainFix->SetUserData((void*)chainUD);

    addSpecRect();
    /*
    Bot *bot;
    bot = new Bot(textures.getTexture(Textures::Type::BOT));
    bot->setBody(addBot(bot));
    AI *ai;
    ai = new AI(player,bot);
    Ai.push_back(ai);
*/

    Car car(world, &textures);


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

    DisplayData* ladderDD = (DisplayData*) new TextureData(textures.getTexture(Textures::Type::LADDER));
    UserData* LadderUD = new Ladder (ladderDD);
    body->SetUserData((void*)LadderUD);
    ladderFixture->SetUserData((void*)LadderUD);

}

void GameWidget::addPlayer (){
    DisplayData* playerDD = (DisplayData*) new TextureData (textures.getTexture(Textures::Type::PLAYER));
    player = new Player(playerDD);
    float playerWidth = 2; float playerHeight = 4;
    b2BodyDef bodydef;
    bodydef.position.Set(0, 10);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    player->setBody(body);
    playerDD->isPlayer = true;
    UserData* playerUD = new UserData (playerDD);
    body->SetUserData((void*) playerUD);
    b2PolygonShape shape;
    shape.SetAsBox(playerWidth/2.0f, playerHeight/2.0f);

    b2FixtureDef fixturedef;
    // fixturedef.friction = 5;
    fixturedef.shape = &shape;
    fixturedef.density = 1.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);

    DisplayData* bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0));
    UserData* bodyUD = new BodyPart(playerDD, BodyPart::Type::BODY);

    mainFixture->SetUserData((void*) bodyUD);

    b2PolygonShape sensorShape;
    sensorShape.SetAsBox(playerWidth/2*0.7, 0.1, b2Vec2(0,-2), 0);
    fixturedef.shape = &sensorShape;
    fixturedef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);

    UserData* footUD = new BodyPart(bodyDD, BodyPart::Type::FOOT_SENSOR);

    footSensorFixture->SetUserData((void*) footUD);
    //hips
    bodydef.position.Set(0, -0.6);
    b2Body* leftHip = world->CreateBody(&bodydef);
    shape.SetAsBox(0.25f/2.0f, 0.5f/2.0f);
    //shins
    bodydef.position.Set(0, -1.1);
    b2Body* leftShin = world->CreateBody(&bodydef);
    shape.SetAsBox(0.25f/2.0f, 0.5f/2.0f);
    //joints
    b2WeldJointDef wjd;
    wjd.Initialize(body, leftHip, b2Vec2(0, -0.5f));
    world->CreateJoint(&wjd);

    wjd.Initialize(leftHip, leftShin, b2Vec2(0, -1.0f));
    world->CreateJoint(&wjd);
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

b2Body *GameWidget::addBot(Bot* bot) {/*
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    DisplayData* bodyDD = new NonDrawable;
    UserData* bodyUD = new UserData (bodyDD);
    body->SetUserData((void*) bodyUD);
    b2PolygonShape shape;
    shape.SetAsBox(1,2);


    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);

    DisplayData* dispData = (DisplayData*)TextureData(textures.getTexture(Textures::Type::));
    UserData* userData = new BodyPart(dispData, BodyPart::Type::BODY);

   mainFixture->SetUserData((void*) new BodyPart (BodyPart::Type::BODY));

    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(0.3, 0.3, b2Vec2(0,-2), 0);
    fixturedef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);

    footSensorFixture->SetUserData((void*) new BodyPart (BodyPart::Type::FOOT_SENSOR));
    return body;*/
}

void GameWidget::updateGame(){
    player->update(&textures);
    if (player->jumpCooldown) --player->jumpCooldown;
    /*
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
*/
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

    glLoadIdentity();

    //background
    TextureData* backgroundTD = new TextureData (textures.getTexture(Textures::Type::BACKGROUND));
    b2Vec2 pointsBackground[4] = {b2Vec2(-WIDTH*P2M,-WIDTH*P2M),b2Vec2(WIDTH*P2M,-WIDTH*P2M),
                                  b2Vec2(WIDTH*P2M,WIDTH*P2M),b2Vec2(-WIDTH*P2M,WIDTH*P2M)};
    drawPolygon (pointsBackground, 4, b2Vec2(0, 0), 0, backgroundTD);

    //bodies

    b2Body* tmp=world->GetBodyList();

    while(tmp)
    {
        b2Fixture* curFixture = tmp->GetFixtureList();
        while(curFixture){
            b2Shape::Type curShapeType = curFixture->GetShape()->GetType();
            UserData* userData = (UserData*)curFixture->GetUserData();
            DisplayData* displayData = userData->displayData;
            if (curShapeType ==  b2Shape::e_polygon){
                int count = ((b2PolygonShape*)curFixture->GetShape())->GetVertexCount();
                b2Vec2 points[count];
                for(int i=0; i < count; i++){
                    points[i]=((b2PolygonShape*)curFixture->GetShape())->GetVertex(i);
                }
                NonDrawable* ND_p = dynamic_cast<NonDrawable*>(displayData);
                TextureData* TD_p = dynamic_cast<TextureData*>(displayData);
                KeyLineData* KLD_p = dynamic_cast<KeyLineData*>(displayData);
                if (!ND_p)
                    if (TD_p){
                        drawPolygon(points, count, tmp->GetPosition(), tmp->GetAngle(), TD_p);
                        TD_p->changeFrame();
                    }
                    else
                        if (KLD_p)
                            drawPolygon(points, count, tmp->GetPosition(), tmp->GetAngle(), KLD_p);
            }
            else
                if (curShapeType == b2Shape::e_circle){
                    KeyLineData* KLD_p = (KeyLineData*) displayData;
                    drawCircle(((b2CircleShape*)curFixture->GetShape())->m_radius, tmp->GetWorldCenter(), KLD_p);
                }
                else
                    if (curShapeType == b2Shape::e_chain){
                        KeyLineData* KLD_p = (KeyLineData*) displayData;
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

                        drawChain (points, tmp->GetWorldCenter(), edgeCount + 1, KLD_p);

                    }
            curFixture=curFixture->GetNext();
        }
        tmp=tmp->GetNext();

    }
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
    if (key == Qt::Key_Space)
        player->isJumping = true;
    if (key == Qt::Key_E)
        player->useObject();
    if (key == Qt::Key_Escape) this->close();
    if (key == Qt::Key_X) {
        player->vehicle->motor->SetMaxMotorTorque(
                    player->vehicle->motor->GetMaxMotorTorque()*1.2);
        qDebug()<<player->vehicle->motor->GetMaxMotorTorque();
    }
    if (key == Qt::Key_Z) {
        player->vehicle->motor->SetMaxMotorTorque(
                    player->vehicle->motor->GetMaxMotorTorque()*1.0f/1.2);
        qDebug()<<player->vehicle->motor->GetMaxMotorTorque();
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
    int key = event->key();
    if ( (key == Qt::Key_Left || key == Qt::Key_A) && player->moveState == Player::MS_LEFT
         ||(key == Qt::Key_Right || key == Qt::Key_D) && player->moveState == Player::MS_RIGHT )
        player->moveState = Player::MS_STAND;
    if ( (key == Qt::Key_Up || key == Qt::Key_W) && player->moveStateVertical == Player::MSV_UP
         ||(key == Qt::Key_Down || key == Qt::Key_S) && player->moveStateVertical == Player::MSV_DOWN )
        player->moveStateVertical = Player::MSV_STAND;
    if ( (key == Qt::Key_Space) && player->isJumping)
        player->isJumping = false;
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

    b2Fixture* bodyFix = body->CreateFixture(&fixturedef);

    DisplayData* bodyDD = (DisplayData*) new TextureData(textures.getTexture(type));
    UserData* bodyUD = new UserData (bodyDD);
    bodyFix->SetUserData((void*) bodyUD);
    body->SetUserData((void*) bodyUD);
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

    b2Fixture* bodyFix = body->CreateFixture(&fixturedef);

    body->ApplyLinearImpulse(b2Vec2(-body->GetMass() * 100, 0), b2Vec2(0, 0), true);

    DisplayData* bodyDD = (DisplayData*) new TextureData(textures.getTexture(Textures::Type::CRATE));
    UserData* bodyUD = new UserData (bodyDD);
    bodyFix->SetUserData((void*) bodyUD);
    body->SetUserData((void*) bodyUD);
    return body;
}

void GameWidget::drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, TextureData* textureData) {

    //setting texture's points

    struct point {float x; float y;};
    point  texPoints [4];
    point  straightImage [4] = {{0.0f, 0.0f},{1.0f, 0.0f},{1.0f, 1.0f},{0.0f, 1.0f}};
    point  mirroredImage [4] = {{1.0f, 0.0f},{0.0f, 0.0f},{0.0f, 1.0f},{1.0f, 1.0f}};
    if (textureData->isMirrored)
        for (int i = 0; i<4; i++)
            texPoints[i] = mirroredImage[i];
    else
        for (int i = 0; i<4; i++)
            texPoints[i] = straightImage[i];

    //choosing sprite tile

    int tileColumn = textureData->currentFrameN % textureData->texture_p->columns;
    int tileRow = textureData->currentFrameN / textureData->texture_p->columns;
    for (int i = 0; i < 4; ++i){

        texPoints[i].x /= textureData->texture_p->columns;
        texPoints[i].y /= textureData->texture_p->rows;
        texPoints[i].x += (float) tileColumn/textureData->texture_p->columns;
        texPoints[i].y += (float) tileRow/textureData->texture_p->rows;
    }

    //drawing texture

    glPushMatrix();
    glColor3f(1, 1, 1);

    if(!textureData->isPlayer)
        glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);

    glRotatef(angle*180.0/M_PI,0,0,1);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textureData->texture_p->id);

    glBegin(GL_POLYGON);
    for(int i = 0; i < count; i++){
        glTexCoord2f(texPoints[i].x, texPoints[i].y);
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void GameWidget::drawChain(b2Vec2* points, b2Vec2 center, int count, KeyLineData *keyLineData) {

    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    glPushMatrix();
    if(!keyLineData->isPlayer)
        glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);

    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < count; i++)
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, KeyLineData *keyLineData) {
    glPushMatrix();
    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    if(!keyLineData->isPlayer)
        glTranslatef(center.x*M2P/WIDTH-player->body->GetWorldCenter().x*M2P/WIDTH,center.y*M2P/WIDTH-player->body->GetWorldCenter().y*M2P/WIDTH,0);
    //glTranslatef (center.x*M2P/WIDTH, center.y*M2P/WIDTH, 0);
    glRotatef(angle*180.0/M_PI, 0, 0, 1);
    glBegin(GL_POLYGON);
    for(int i=0;i<count;i++)
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawCircle(float radius, b2Vec2 center, KeyLineData *keyLineData){
    glPushMatrix();
    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    if(!keyLineData->isPlayer)
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

