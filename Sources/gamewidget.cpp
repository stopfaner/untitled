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
    //addWalkingMachine();
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

        //
        dx = 15;
        dy = 0;
        //


        x += dx;
        if (i>0){
            vs[i].Set(x - dx / 2, ((vs[i-1].y + (dy / 1.5f -15.0f)) /2));
            ++i;
        }
        vs[i].Set(x, dy - 15.0f);
        ++i;
    }
    vs[0].x = -1000;
    vs[i].x = 1000;


    b2ChainShape chainShape;

    chain->SetUserData((void*) new UserData);

    chainShape.CreateChain(vs, i);

    b2FixtureDef chainFixtureDef;
    chainFixtureDef.shape = &chainShape;
    chainFixtureDef.density = 1.0;

    b2Fixture* chainFix = chain->CreateFixture(&chainFixtureDef);

    DisplayData* chainDD = (DisplayData*) new KeyLineData (Color(0, 0, 255), DisplayData::Layer::LANDSCAPE);
    chainFix->SetUserData((void*) new UserData (new GameObject, chainDD));

    addSpecRect();

    new Car (world, &textures);


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

    body->SetUserData((void*) new UserData);
    DisplayData* ladderDD = (DisplayData*) new TextureData(textures.getTexture(Textures::Type::LADDER),
                                                           DisplayData::Layer::BACKGROUND_OBJECT);
    ladderFixture->SetUserData((void*) new UserData(new Ladder, ladderDD));


    //interface
    displayItems.push_back(new HUDElement (new TextureData(textures.getTexture(Textures::Type::TEST2), DisplayData::Layer::HUD),
                                           b2Vec2(5, -5), b2Vec2(1, 1), 0));

}

void GameWidget::addWalkingMachine (){
    float x = 20, y = 5;
    float widthScale = 1, heightScale = 1;
    DisplayData* DD = (DisplayData*) new KeyLineData (Color(100, 150, 50), DisplayData::Layer::OBJECT);

    b2Body* mainBody = addRect(x, y, 3 * widthScale * 2, 0.5 * heightScale * 2, true, Textures::Type::TEST1);
    mainBody->SetType(b2_staticBody);
    b2Filter filter;
    filter.groupIndex = -1;
    mainBody->GetFixtureList()->SetFilterData(filter);
    b2BodyDef bodydef;
    bodydef.position.Set(x, y);
    bodydef.type = b2_dynamicBody;




    UserData* basicUD = new UserData(DD);


    b2Body* circleBody = world->CreateBody(&bodydef);
//circleBody->SetType(b2_staticBody);
    b2CircleShape circleShape;
    circleShape.m_radius = 1;

    b2FixtureDef fixturedef;
    fixturedef.density = 1;
    fixturedef.shape = &circleShape;
    fixturedef.filter.groupIndex = -1;
    b2Fixture* circleFixture = circleBody->CreateFixture(&fixturedef);
    circleFixture->SetUserData(static_cast<void*> (basicUD));


    b2RevoluteJointDef mainRJD;
    mainRJD.Initialize(mainBody, circleBody, circleBody->GetWorldCenter());
    mainRJD.maxMotorTorque = 300;
    mainRJD.motorSpeed = 5;
    mainRJD.enableMotor = true;
    b2RevoluteJoint* mainRJ = static_cast<b2RevoluteJoint*> (world->CreateJoint(&mainRJD));
mainRJD.enableMotor = false;

    b2EdgeShape edgeShape;
    edgeShape.Set(b2Vec2(0, 0), b2Vec2(-4 * widthScale, 3 * heightScale ));
    bodydef.position.Set(x + circleShape.m_radius, y);
    b2Body* edge=world->CreateBody(&bodydef);
//edge->SetType(b2_staticBody);
    fixturedef.shape = &edgeShape;

    b2Fixture* fixture = edge->CreateFixture(&fixturedef);
    fixture->SetUserData(static_cast<void*>(new UserData
                                            (new KeyLineData(Color(10, 90, 113), DisplayData::Layer::OBJECT))));
    mainRJD.Initialize(circleBody, edge, edge->GetPosition());
    world->CreateJoint(&mainRJD);

    /*
    b2Body* plank1 = addRect(x - 2.5 * widthScale - circleShape.m_radius, y, 2.5 * widthScale * 2, 0.05 * heightScale * 2, true, Textures::Type::TEST1);
plank1->SetType(b2_staticBody);
    plank1->GetFixtureList()->SetFilterData(filter);
    mainRJD.Initialize(circleBody, plank1, b2Vec2(circleBody->GetWorldCenter().x - circleShape.m_radius,
                                                  circleBody->GetWorldCenter().y));
    mainRJD.enableMotor = false;
    world->CreateJoint(&mainRJD);
*/


    b2PolygonShape triangleShape;
    b2Vec2 trianglePoints [3] = {b2Vec2(0, 0), b2Vec2(-3 * widthScale, 0), b2Vec2(0, 3 * heightScale)};
    triangleShape.Set(trianglePoints, 3);




    bodydef.position.Set(x - 3 * widthScale, y);
    bodydef.type=b2_dynamicBody;
    b2Body* triangle=world->CreateBody(&bodydef);
//triangle->SetType(b2_staticBody);
    fixturedef.shape = &triangleShape;

    fixture = triangle->CreateFixture(&fixturedef);
    fixture->SetUserData(static_cast<void*>(new UserData
                                            (new KeyLineData(Color(10, 90, 113), DisplayData::Layer::OBJECT))));


    mainRJD.Initialize(mainBody, triangle, triangle->GetPosition());
    world->CreateJoint(&mainRJD);

    mainRJD.Initialize(triangle, edge, b2Vec2(triangle->GetPosition().x, triangle->GetPosition().y + 3 * heightScale));
    world->CreateJoint(&mainRJD);



    edgeShape.Set(b2Vec2(0, 0), b2Vec2(0, - 3 * heightScale ));
    bodydef.position.Set(triangle->GetPosition().x, triangle->GetPosition().y);
    b2Body* edge1=world->CreateBody(&bodydef);
//edge1->SetType(b2_staticBody);
    fixturedef.shape = &edgeShape;

    fixture = edge1->CreateFixture(&fixturedef);
    fixture->SetUserData(static_cast<void*>(new UserData
                                            (new KeyLineData(Color(10, 90, 113), DisplayData::Layer::OBJECT))));
    mainRJD.Initialize(triangle, edge1, triangle->GetPosition());
    world->CreateJoint(&mainRJD);


    bodydef.position.Set(triangle->GetPosition().x - 3 * widthScale, triangle->GetPosition().y);
    b2Body* edge2=world->CreateBody(&bodydef);
//edge2->SetType(b2_staticBody);
    fixturedef.shape = &edgeShape;

    fixture = edge2->CreateFixture(&fixturedef);
    fixture->SetUserData(static_cast<void*>(new UserData
                                            (new KeyLineData(Color(10, 90, 113), DisplayData::Layer::OBJECT))));
    mainRJD.Initialize(triangle, edge2, b2Vec2(triangle->GetPosition().x  - 3 * widthScale, triangle->GetPosition().y));
    world->CreateJoint(&mainRJD);



    trianglePoints[0] = b2Vec2(0, 0);
    trianglePoints[1] = b2Vec2(-3 * widthScale, 0);
    trianglePoints[2] = b2Vec2(0, -4 * heightScale);

    triangleShape.Set(trianglePoints, 3);
    bodydef.position.Set(triangle->GetPosition().x, triangle->GetPosition().y - 3 * heightScale);
    b2Body* triangle2=world->CreateBody(&bodydef);
//triangle2->SetType(b2_staticBody);
    fixturedef.shape = &triangleShape;

    fixture = triangle2->CreateFixture(&fixturedef);
    fixture->SetUserData(static_cast<void*>(new UserData
                                            (new KeyLineData(Color(10, 90, 113), DisplayData::Layer::OBJECT))));

    mainRJD.Initialize(edge1, triangle2, b2Vec2(triangle->GetPosition().x, triangle->GetPosition().y - 3 * heightScale));
    world->CreateJoint(&mainRJD);

    mainRJD.Initialize(edge2, triangle2, b2Vec2(triangle->GetPosition().x  - 3 * widthScale, triangle->GetPosition().y - 3 * heightScale));
    world->CreateJoint(&mainRJD);



    /*
    bodydef.position.Set(triangle->GetPosition().x - 3 * widthScale, triangle->GetPosition().y);
    b2Body* edge3=world->CreateBody(&bodydef);
edge3->SetType(b2_staticBody);
    fixturedef.shape = &edgeShape;
    fixturedef.density = 1.0;
    fixturedef.filter.groupIndex = -1;

    fixture = edge3->CreateFixture(&fixturedef);
    fixture->SetUserData(static_cast<void*>(new UserData
                                            (new KeyLineData(Color(10, 90, 113), DisplayData::Layer::OBJECT))));
    mainRJD.Initialize(triangle, edge3, b2Vec2(triangle->GetPosition().x  - 3 * widthScale, triangle->GetPosition().y));
    world->CreateJoint(&mainRJD);
    */

    mainRJD.Initialize(triangle2, circleBody, b2Vec2(circleBody->GetPosition().x  + circleShape.m_radius, triangle->GetPosition().y));
    world->CreateJoint(&mainRJD);
    mainRJD.Initialize(triangle2, circleBody, b2Vec2(triangle2->GetPosition().x, triangle2->GetPosition().y));
    world->CreateJoint(&mainRJD);

}

void GameWidget::addPlayer (){
    DisplayData* playerDD = (DisplayData*) new KeyLineData (Color(), DisplayData::Layer::PLAYER);
    player = new Player(playerDD);
    float playerWidth = 1; float playerHeight = 6;
    b2BodyDef bodydef;
    bodydef.position.Set(20, 10);
    bodydef.type = b2_dynamicBody;
    //bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    player->setBody(body);
    //  playerDD->isShifting = false;

    body->SetUserData((void*) new UserData(player, playerDD));
    b2PolygonShape shape;
    b2Vec2 points[4] = {b2Vec2(- playerWidth * 1 /2.0f, playerHeight * 0.4 / 2.0f), b2Vec2(playerWidth * 1 /2.0f, playerHeight * 0.4 / 2.0f),
                        b2Vec2(- playerWidth * 1 /3.0f, -playerHeight * 0.4 / 2.0f), b2Vec2(playerWidth * 1 /3.0f, -playerHeight * 0.4 / 2.0f)};
    shape.Set(points, 4);
    //shape.SetAsBox(playerWidth * 1 / 2.0f, playerHeight * 0.4 / 2.0f, b2Vec2(0, 0), 0);

    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.filter.categoryBits = CollisionType::BODYPART;
    fixturedef.filter.maskBits = CollisionType::BASIC;
    fixturedef.density = 0.7;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);

    DisplayData* bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);

    BodyPart* BPBody = new BodyPart(player, BodyPart::Type::BODY, body);
    GameObject* bodyPart = BPBody;

    mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                       textures.getTexture(Textures::Type::BODY), DisplayData::Layer::PLAYER)));

    ///


    player->bodyParts = BodyParts ();
    float motorTorque = body->GetMass() * 13;

    //leg

    b2RevoluteJoint* RJhip, *RJhip2, *RJknee, *RJknee2, *RJfoot, *RJfoot2;
    for (int i = 0; i < 2; ++i){
        b2BodyDef bodydefHip;
        bodydefHip.position.Set(20, 10 - playerHeight * (0.4 + 0.2) / 2.0f);
        bodydefHip.type = b2_dynamicBody;
        bodydefHip.fixedRotation = false;
        b2Body* hip = world->CreateBody(&bodydefHip);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = hip->CreateFixture(&fixturedef);
        if (i)
            bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);
        else
            bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 0), DisplayData::Layer::PLAYER);

        BodyPart* BPHip = new BodyPart(player, BodyPart::Type::HIP, hip);
        bodyPart = BPHip;

        DisplayData::Layer layer;
        if (i) layer = DisplayData::Layer::PLAYER_FAR;
        else layer = DisplayData::Layer::PLAYER_NEAR;
        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures.getTexture(Textures::Type::HIP), layer)));


        b2RevoluteJointDef RJDhip;
        RJDhip.enableLimit = true;
        RJDhip.upperAngle = M_PI * 0.9;
        RJDhip.lowerAngle = - M_PI / 6;
        RJDhip.maxMotorTorque = motorTorque * 2.0f;
        RJDhip.Initialize(body, hip, b2Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y - playerHeight * 0.4 / 2.0f));
        b2RevoluteJoint* RJhipTemp = static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDhip));
        RJhipTemp->EnableMotor(true);


        float offset = 0.05;

        b2BodyDef bodydefShin;
        bodydefShin.position.Set(20, 10 - playerHeight * ( (0.4 + 0.2) / 2.0f + 0.2 - offset  ));
        bodydefShin.type = b2_dynamicBody;
        bodydefShin.fixedRotation = false;
        b2Body* shin = world->CreateBody(&bodydefShin);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = shin->CreateFixture(&fixturedef);

        BodyPart* BPShin = new BodyPart(player, BodyPart::Type::SHIN, shin);
        bodyPart = BPShin;


        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures.getTexture(Textures::Type::SHIN), layer)));


        b2RevoluteJointDef RJDknee;
        RJDknee.enableLimit = true;
        RJDknee.upperAngle = 0;
        RJDknee.lowerAngle = - M_PI / 2;
        RJDknee.maxMotorTorque = motorTorque * 1.5;
        RJDknee.Initialize(hip, shin, b2Vec2(hip->GetWorldCenter().x, hip->GetWorldCenter().y - playerHeight * (0.2 - offset) / 2.0f));
        b2RevoluteJoint* RJkneeTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDknee));
        RJkneeTemp->EnableMotor(true);



        b2BodyDef bodydefFoot;
        bodydefFoot.position.Set(20 + 0.7 / 2.0f, 10 - playerHeight * ( (0.4 / 2.0f + 0.2 + 0.2 - offset - 0.025 / 2  )));
        bodydefFoot.type = b2_dynamicBody;
        bodydefFoot.fixedRotation = false;
        b2Body* foot = world->CreateBody(&bodydefFoot);
        shape.SetAsBox(playerWidth * 0.8 / 2.0f, playerHeight * 0.025 / 2.0f);
        fixturedef.friction = 1.15;
        fixturedef.shape = &shape;

        mainFixture = foot->CreateFixture(&fixturedef);
        fixturedef.friction = 1;
        bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 255), DisplayData::Layer::PLAYER);


        BodyPart* BPFoot = new BodyPart(player, BodyPart::Type::FOOT, foot);
        bodyPart = BPFoot;

        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures.getTexture(Textures::Type::FOOT), layer)));

        b2RevoluteJointDef RJDfoot;
        RJDfoot.enableLimit = true;
        RJDfoot.upperAngle = 1;
        RJDfoot.lowerAngle = -1;
        RJDfoot.maxMotorTorque = motorTorque;
        RJDfoot.Initialize(shin, foot, b2Vec2(foot->GetWorldCenter().x - 0.6 / 2.0f, foot->GetWorldCenter().y));
        b2RevoluteJoint* RJfootTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDfoot));
        RJfootTemp->EnableMotor(true);

        BPHip->RJ = RJhipTemp;
        BPShin->RJ = RJkneeTemp;
        BPFoot->RJ = RJfootTemp;

        if (i){
            RJfoot = RJfootTemp;
            RJknee = RJkneeTemp;
            RJhip = RJhipTemp;
            player->bodyParts.hip = BPHip;
            player->bodyParts.shin = BPShin;
            player->bodyParts.foot = BPFoot;
        }
        else{
            RJfoot2 = RJfootTemp;
            RJknee2 = RJkneeTemp;
            RJhip2 = RJhipTemp;
            player->bodyParts.hip2 = BPHip;
            player->bodyParts.shin2 = BPShin;
            player->bodyParts.foot2 = BPFoot;
        }
    }
    //

    b2BodyDef bodydefHead;
    bodydefHead.position.Set(20, 10 + playerHeight * (0.4 / 2.0f) + playerWidth * 0.5);
    bodydefHead.type = b2_dynamicBody;
    //bodydefHead.fixedRotation = false;
    b2Body* head = world->CreateBody(&bodydefHead);
    b2CircleShape circleShape;
    circleShape.m_radius = playerWidth * 0.5;

    fixturedef.shape = &circleShape;

    mainFixture = head->CreateFixture(&fixturedef);

    bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 100), DisplayData::Layer::PLAYER);

    BodyPart* BPHead = new BodyPart(player, BodyPart::Type::HEAD, head);
    bodyPart = BPHead;

    mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                       textures.getTexture(Textures::Type::HEAD), DisplayData::Layer::PLAYER)));


    b2RevoluteJointDef RJDhead;
    RJDhead.enableLimit = true;
    RJDhead.upperAngle = 0.1;
    RJDhead.lowerAngle = -0.1;
    RJDhead.maxMotorTorque = motorTorque;
    RJDhead.Initialize(body, head, b2Vec2(head->GetWorldCenter().x, head->GetWorldCenter().y - playerWidth * 0.5));
    world->CreateJoint(&RJDhead);


    player->bodyParts.body = BPBody;
    player->bodyParts.body = BPHead;

    ///

    //arms
    b2RevoluteJoint* RJshoulder, *RJshoulder2, *RJforearm, *RJforearm2, *RJwrist, *RJwrist2;
    for (int i = 0; i < 2; ++i){
        b2BodyDef bodydefshoulder;
        bodydefshoulder.position.Set(20, 10 + playerHeight * (0.4 - 0.2) / 2.0f);
        bodydefshoulder.type = b2_dynamicBody;
        bodydefshoulder.fixedRotation = false;
        b2Body* shoulder = world->CreateBody(&bodydefshoulder);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = shoulder->CreateFixture(&fixturedef);
        DisplayData::Layer layer;
        if (i){
            layer = DisplayData::Layer::PLAYER_FAR;
            bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), DisplayData::Layer::PLAYER);
        }
            else{
            bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 0), DisplayData::Layer::PLAYER);
            layer = DisplayData::Layer::PLAYER_NEAR;
        }
        BodyPart* BPshoulder = new BodyPart(player, BodyPart::Type::SHOULDER, shoulder);
        bodyPart = BPshoulder;


        mainFixture->SetUserData((void*) new UserData (bodyPart,  new TextureData(
                                                           textures.getTexture(Textures::Type::SHOULDER), layer)));


        b2RevoluteJointDef RJDshoulder;
        RJDshoulder.enableLimit = true;
        RJDshoulder.upperAngle = M_PI;
        RJDshoulder.lowerAngle = - M_PI / 2;
        RJDshoulder.maxMotorTorque = motorTorque;
        RJDshoulder.Initialize(body, shoulder, b2Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y + playerHeight * 0.4 / 2.0f));
        b2RevoluteJoint* RJshoulderTemp = static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDshoulder));
        RJshoulderTemp->EnableMotor(true);


        float offset = 0.1;

        b2BodyDef bodydefforearm;
        // bodydefforearm.position.Set(20, 10 - playerHeight * ( (0.4 + 0.2) / 2.0f + 0.2 - offset  ));
        bodydefforearm.position.Set(20, 10 - playerHeight * (0.2 / 2.0f));
        bodydefforearm.type = b2_dynamicBody;
        bodydefforearm.fixedRotation = false;
        b2Body* forearm = world->CreateBody(&bodydefforearm);
        shape.SetAsBox(playerWidth * 0.4 / 2.0f, playerHeight * 0.2 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = forearm->CreateFixture(&fixturedef);

        BodyPart* BPforearm = new BodyPart(player, BodyPart::Type::FOREARM, forearm);
        bodyPart = BPforearm;


        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures.getTexture(Textures::Type::FOREARM), layer)));


        b2RevoluteJointDef RJDforearm;
        RJDforearm.enableLimit = true;
        RJDforearm.upperAngle = M_PI;
        RJDforearm.lowerAngle = 0;
        RJDforearm.maxMotorTorque = motorTorque;
        //RJDforearm.Initialize(shoulder, forearm, b2Vec2(shoulder->GetWorldCenter().x, shoulder->GetWorldCenter().y - playerHeight * (0.2 - offset) / 2.0f));
        RJDforearm.Initialize(shoulder, forearm, b2Vec2(shoulder->GetWorldCenter().x, body->GetWorldCenter().y));

        b2RevoluteJoint* RJforearmTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDforearm));
        RJforearmTemp->EnableMotor(true);



        b2BodyDef bodydefwrist;
        //bodydefwrist.position.Set(20, 10 - playerHeight * ( (0.4 / 2.0f + 0.2 + 0.2 - offset - 0.025 / 2  )));
        bodydefwrist.position.Set(20, 10 - playerHeight * (0.4 / 2.0f));
        bodydefwrist.type = b2_dynamicBody;
        bodydefwrist.fixedRotation = false;
        b2Body* wrist = world->CreateBody(&bodydefwrist);
        shape.SetAsBox(playerHeight * 0.025 / 2.0f, playerWidth * 0.9 / 2.0f);

        fixturedef.shape = &shape;

        mainFixture = wrist->CreateFixture(&fixturedef);

        bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 255), DisplayData::Layer::PLAYER);


        BodyPart* BPwrist = new BodyPart(player, BodyPart::Type::WRIST, wrist);
        bodyPart = BPwrist;

        mainFixture->SetUserData((void*) new UserData (bodyPart, new TextureData(
                                                           textures.getTexture(Textures::Type::WRIST), layer)));

        b2RevoluteJointDef RJDwrist;
        RJDwrist.enableLimit = true;
        RJDwrist.upperAngle = 1;
        RJDwrist.lowerAngle = -1;
        RJDwrist.maxMotorTorque = motorTorque;
        //RJDwrist.Initialize(forearm, wrist, b2Vec2(wrist->GetWorldCenter().x - 0.9 / 2.0f, wrist->GetWorldCenter().y));
        RJDwrist.Initialize(forearm, wrist, b2Vec2(wrist->GetWorldCenter().x, wrist->GetWorldCenter().y + playerWidth * 0.9 / 2.0f));
        b2RevoluteJoint* RJwristTemp =  static_cast<b2RevoluteJoint*> (world->CreateJoint(&RJDwrist));
        RJwristTemp->EnableMotor(true);

        BPshoulder->RJ = RJshoulderTemp;
        BPforearm->RJ = RJforearmTemp;
        BPwrist->RJ = RJwristTemp;

        if (i){
            RJwrist = RJwristTemp;
            RJforearm = RJforearmTemp;
            RJshoulder = RJshoulderTemp;
            player->bodyParts.shoulder = BPshoulder;
            player->bodyParts.forearm = BPforearm;
            player->bodyParts.wrist = BPwrist;
        }
        else{
            RJwrist2 = RJwristTemp;
            RJforearm2 = RJforearmTemp;
            RJshoulder2 = RJshoulderTemp;
            player->bodyParts.shoulder2 = BPshoulder;
            player->bodyParts.forearm2 = BPforearm;
            player->bodyParts.wrist2 = BPwrist;
        }
    }
    //
    /*

            b2BodyDef bodydef2;
            bodydef2.position.Set(20, 10);
            bodydef2.type = b2_dynamicBody;
            bodydef2.fixedRotation = false;
            b2Body* body2 = world->CreateBody(&bodydef2);
            shape.SetAsBox(playerWidth/8.0f, playerHeight/4.0f, b2Vec2(0, -playerHeight/6), 0);

            fixturedef.shape = &shape;

            mainFixture = body2->CreateFixture(&fixturedef);

            bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0));
             bodyPart = new BodyPart(player, BodyPart::Type::SHIN_RIGHT);

            mainFixture->SetUserData((void*) new UserData (bodyPart, playerDD));


            b2RevoluteJointDef RJD2;
            RJD2.enableLimit = true;
            RJD2.upperAngle = 0.1;
            RJD2.lowerAngle = -0.1;
            RJD2.Initialize(body, body2, b2Vec2(body->GetWorldCenter().x, body->GetWorldCenter().y - playerHeight/4.0f));
            world->CreateJoint(&RJD2);

            b2RevoluteJointDef shins;
            shins.Initialize(body1, body2, body1->GetWorldCenter());
        */

    /*

            b2PolygonShape sensorShape;
            sensorShape.SetAsBox(playerWidth/2*0.7, 0.1, b2Vec2(0,-2), 0);
            fixturedef.shape = &sensorShape;
            fixturedef.isSensor = true;
            b2Fixture* footSensorFixture = body->CreateFixture(&fixturedef);

            GameObject* footPart = new BodyPart(player, BodyPart::Type::FOOT_SENSOR, );

            footSensorFixture->SetUserData((void*) new UserData (footPart, bodyDD));
        */
}

b2Body *GameWidget::addBot(Bot* bot) {/*
    b2BodyDef bodydef;
    bodydef.position.Set(0, 0);
    bodydef.type = b2_dynamicBody;
    bodydef.fixedRotation = true;
    b2Body* body = world->CreateBody(&bodydef);
    DisplayData* bodyDD = new NonDrawable;
    GameObject* bodyUD = new GameObject (bodyDD);
    body->SetUserData((void*) bodyUD);
    b2PolygonShape shape;
    shape.SetAsBox(1,2);


    b2FixtureDef fixturedef;
    fixturedef.shape = &shape;
    fixturedef.density = 0.0;

    b2Fixture* mainFixture = body->CreateFixture(&fixturedef);

    DisplayData* dispData = (DisplayData*)TextureData(textures.getTexture(Textures::Type::));
    GameObject* GameObject = new BodyPart(dispData, BodyPart::Type::BODY);

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
    glEnable(GL_DEPTH_TEST);
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
    glClear(GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    //background
    TextureData* backgroundTD = new TextureData (textures.getTexture(Textures::Type::BACKGROUND), DisplayData::Layer::BACKGROUND);
    b2Vec2 pointsBackground[4] = {b2Vec2(-WIDTH*P2M,-WIDTH*P2M),b2Vec2(WIDTH*P2M,-WIDTH*P2M),
                                  b2Vec2(WIDTH*P2M,WIDTH*P2M),b2Vec2(-WIDTH*P2M,WIDTH*P2M)};
    drawPolygon (pointsBackground, 4, b2Vec2(0, 0), 0, backgroundTD);

    //bodies

    b2Body* tmp=world->GetBodyList();

    while(tmp)
    {
        b2Fixture* curFixture = tmp->GetFixtureList();
        while(curFixture){
            b2Shape::Type curFixtureType = curFixture->GetShape()->GetType();

            DisplayData* displayData = nullptr;
            void* UD = curFixture->GetUserData();
            if (!UD) qDebug()<<"NO USERDATA";
            else
                displayData = static_cast<UserData*>(UD)->displayData;
            if (displayData){
                if (curFixtureType ==  b2Shape::e_polygon){
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
                    if (curFixtureType == b2Shape::e_circle){
                        TextureData* TD_p = dynamic_cast<TextureData*>(displayData);
                        KeyLineData* KLD_p = dynamic_cast<KeyLineData*> (displayData);
                        if (KLD_p)
                            drawCircle(((b2CircleShape*)curFixture->GetShape())->m_radius,
                                   tmp->GetPosition(), KLD_p, tmp->GetAngle());
                        else
                            if (TD_p){
                                float radius = static_cast<b2CircleShape*>(curFixture->GetShape())->m_radius;
                                drawRectangle(tmp->GetPosition(), radius, radius, tmp->GetAngle(), TD_p);
                            }
                    }
                    else
                        if (curFixtureType == b2Shape::e_chain){
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

                            drawChain (points, tmp->GetPosition(), edgeCount + 1, KLD_p);

                        }
                        else
                            if (curFixtureType == b2Shape::e_edge){
                                KeyLineData* KLD_p = (KeyLineData*) displayData;

                                b2Vec2 points [2];

                                points[0] = static_cast<b2EdgeShape*>(curFixture->GetShape())->m_vertex1;
                                points[1] = static_cast<b2EdgeShape*>(curFixture->GetShape())->m_vertex2;


                                drawChain (points, tmp->GetPosition(), 2, KLD_p);

                            }
            }
            //draw joints

            b2JointEdge* curJoint = tmp->GetJointList();
            while(curJoint){
                drawCircle(0.07, curJoint->joint->GetAnchorA(), new KeyLineData(Color(255,0,0), DisplayData::Layer::JOINT), 0);
                drawCircle(0.07, curJoint->joint->GetAnchorB(), new KeyLineData(Color(255,0,0), DisplayData::Layer::JOINT), 0);
                curJoint = curJoint->next;
            }

            curFixture=curFixture->GetNext();
        }
        tmp=tmp->GetNext();

    }


    //interface
    //DisplayData* displayItem = displayItems.front();
    for (std::list<UIElement*>::const_iterator iterator = displayItems.begin(), end = displayItems.end(); iterator != end; ++iterator) {
        UIElement* item = *iterator;
        if (item)
            drawRectangle(item->center, item->size.x, item->size.y, item->angle, item->textureData);
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
    if (key == Qt::Key_F)
        player->fall();
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

    DisplayData* bodyDD = (DisplayData*) new TextureData(textures.getTexture(type), DisplayData::Layer::OBJECT);

    bodyFix->SetUserData((void*) new UserData(bodyDD));
    body->SetUserData((void*) new UserData);
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

    DisplayData* bodyDD = (DisplayData*) new TextureData(textures.getTexture(Textures::Type::CRATE), DisplayData::Layer::OBJECT);
    bodyFix->SetUserData((void*) new UserData(bodyDD));
    body->SetUserData((void*) new UserData);
    return body;
}

void GameWidget::drawRectangle(b2Vec2 center, float width, float height, float angle, TextureData* textureData){
    b2Vec2 points[4] {b2Vec2(-width,-height),b2Vec2(width,-height),
                b2Vec2(width,height),b2Vec2(-width,height)};
    drawPolygon(points, 4, center, angle, textureData);
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
    glTranslatef(center.x*M2P/WIDTH, center.y*M2P/WIDTH, textureData->layer/ (float) DisplayData::Layer::MAX);
    if(textureData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x*M2P/WIDTH,-player->body->GetWorldCenter().y*M2P/WIDTH, 0);

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
    glTranslatef(center.x*M2P/WIDTH, center.y*M2P/WIDTH, keyLineData->layer/ (float) DisplayData::Layer::MAX);
    if(keyLineData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x*M2P/WIDTH,-player->body->GetWorldCenter().y*M2P/WIDTH, 0);

    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < count; i++)
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, KeyLineData *keyLineData) {
    glPushMatrix();
    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    glTranslatef(center.x*M2P/WIDTH, center.y*M2P/WIDTH, keyLineData->layer/ (float) DisplayData::Layer::MAX);
    if(keyLineData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x*M2P/WIDTH,-player->body->GetWorldCenter().y*M2P/WIDTH, 0);
    //glTranslatef (center.x*M2P/WIDTH, center.y*M2P/WIDTH, 0);
    glRotatef(angle*180.0/M_PI, 0, 0, 1);
    //glBegin(GL_POLYGON);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<count;i++)
        glVertex2f(points[i].x*M2P/WIDTH,points[i].y*M2P/WIDTH);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawCircle(float radius, b2Vec2 center, KeyLineData *keyLineData, float angle){
    glPushMatrix();
    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    glTranslatef(center.x*M2P/WIDTH, center.y*M2P/WIDTH, keyLineData->layer/ (float) DisplayData::Layer::MAX);
    if(keyLineData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x*M2P/WIDTH,-player->body->GetWorldCenter().y*M2P/WIDTH, 0);

    glBegin(GL_LINE_LOOP);

    for (int i=0; i < 360; i++)
    {
        float rad = i*M_PI/180.0f;
        glVertex2f(cos(rad)*radius*M2P/WIDTH, sin(rad)*radius*M2P/WIDTH);
    }

    glEnd();


    glBegin(GL_LINE_LOOP);
    glVertex2f(cos(angle)*radius*M2P/WIDTH / 2.0f, sin(angle)*radius*M2P/WIDTH / 2.0f);
    glVertex2f(0, 0);
    glEnd();


    glPopMatrix();
}

