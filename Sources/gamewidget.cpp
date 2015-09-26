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



float TEXTURE_SIZE = 100.0f;
float GRID_STEP = TEXTURE_SIZE;
float CONVERSION_KOEF = 1000;//clipping precise, may cause overflow

/*
 * TODO list
 * separate paint
 * textdisplyadata
 * TO FIX
 * all lanscape fixtures have common userdata
 * fps with triangle textures
*/
/**
 * @brief GameWidget::GameWidget - contructor
 * @param parent - parent widget instance
 */

GameWidget::GameWidget(QWidget *parent) : QGLWidget(parent) {
    textures = new Textures;
    GeneralInfo::getInstance().textures = textures;
    isTexturesEnabled = false;
    isPaused = false;
    testKeyLineData = new KeyLineData(Color(150, 100, 50), DisplayData::Layer::NEAREST);
}


void GameWidget::createWorld(){

    timer = new QTimer;
    connect (timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(10);

    world = new b2World(b2Vec2(0.0,-9.81));
    GeneralInfo::getInstance().world = world;

    loadBackground();


    b2Vec2 delta (0, 0); // move all bodies by this offset

    string errorMsg;
    b2dJson json;
    json.readFromFile("json/test.json", errorMsg);

    b2FixtureDef fixturedef;
    fixturedef.density = 1.0;
    fixturedef.filter.groupIndex = 1;


    vector <b2Body*> bodies;
    json.getAllBodies(bodies);
    for (int i = 0; i < bodies.size(); ++i){
        bool isKeyLine = false;
        if (isKeyLine)
            Triangulation::triangulateChain(Triangulation::chainToPolyline(bodies.at(i)->GetFixtureList()), fixturedef,
                                            new UserData(new KeyLineData(Color(255, 255, 0), DisplayData::Layer::LANDSCAPE)), delta + bodies[i]->GetPosition(), b2_staticBody);
        else
            Triangulation::triangulateChain (Triangulation::chainToPolyline(bodies.at(i)->GetFixtureList()), fixturedef,
                                             new UserData(new TriangleTextureData
                                                          (textures->getTextureID(Textures::Type::GROUND), DisplayData::Layer::LANDSCAPE)), delta + bodies[i]->GetPosition(), b2_staticBody);
    }

    b2AABB *borderWorld = new b2AABB();
    borderWorld->lowerBound.Set(-1000.0, -1000.0);
    borderWorld->upperBound.Set(1000.0, 1000.0);


    player = new Player ();
    player->constructBody(false, delta.x + 10, delta.y + 20);

    unsigned int NPCcount = 0;
    for (unsigned int i = 0; i < NPCcount; ++i){
        NPC* npc = new NPC;
        npc->constructBody(false, 20 + 3 * i, 100);
    }
    new Ladder (b2Vec2(-20, -5), b2Vec2(2, 10));

    //addWalkingMachine();
    addRect(0, -1000, 1000, 2, false, Textures::Type::WALL);
    addRect(0, 1000, 1000, 2, false, Textures::Type::WALL);
    addRect(-1000, 0, 2, 1000, false, Textures::Type::WALL);
    addRect(1000, 0, 2, 10000, false, Textures::Type::WALL);
    // addRect(0, -260, 500, 500, false, Textures::Type::WALL);

    //creating chain
    /*
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


        //dx = 15;
        //dy = 0;



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
*/
    //


    //  addSpecRect();

    new Car (b2Vec2(-50, 10), 0.5);

    //Build build();
    // build.generateDungeon(b2Vec2(0, 70), 5, 5);


    //interface
    displayItems.push_back(new HUDElement (new TextureData(textures->getTextureID(Textures::Type::TEST1), DisplayData::Layer::HUD),
                                           b2Vec2(20, -5), b2Vec2(2, 2), 0));

}

void GameWidget::updateGame(){
    for (std::list<Entity*>::const_iterator iterator = EntityList::getInstance().list.begin(),
         end = EntityList::getInstance().list.end(); iterator != end; ++iterator) {
        Entity* item = *iterator;
        item->update();
    }
    if (destroyBodies.size())
        destroyLandscape();
}

void GameWidget::initializeGL() {

    resize(WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0,0,0,1);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_ALPHA_TEST);
    textures->loadAll();
    createWorld();
}

void GameWidget::resizeGL(int nWidth, int nHeight) {
    WIDTH = nWidth;
    HEIGHT = nHeight;
    glViewport(0, 0, nWidth, nHeight); // установка точки обзора
    glMatrixMode(GL_PROJECTION); // установка режима матрицы
    glLoadIdentity(); // загрузка матрицы
}

void GameWidget::loadBackground (){

    TextureData* backgroundTD = new TextureData (textures->getTextureID(Textures::Type::BACKGROUND), DisplayData::Layer::BACKGROUND);

    displayItems.push_back(new UIElement (backgroundTD, b2Vec2(0, 0), b2Vec2 (80, 40), 0, true));
}

void GameWidget::paintGL() {
    bool drawindEnabled = true;
    if (drawindEnabled){
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        //TODO move in dispItems, change dispItems to DisplayData vec, do cords in meters
        //drawNumber(42.42, 0, 10, 1, 5);
        drawText("hello, world!", b2Vec2(-10, 10), 0.5);
        //interface
        for (std::list<UIElement*>::const_iterator iterator = displayItems.begin(), end = displayItems.end(); iterator != end; ++iterator) {
            UIElement* item = *iterator;
            if (item)
                drawRectangle(item->center, item->size.x, item->size.y, item->angle, item->textureData);
        }

        //bodies
        b2Body* tmp=world->GetBodyList();
        while(tmp){
            UserData* UD = static_cast<UserData*>(tmp->GetUserData());
            DisplayData* displayData = UD->displayData;
            BodyTextureData* BTD = dynamic_cast<BodyTextureData*>(displayData);
            if (BTD && isTexturesEnabled)
                drawRectangleFull(BTD->centerBody->GetPosition(), BTD->width, BTD->height, tmp->GetAngle(), BTD);
                else{
                b2Fixture* curFixture = tmp->GetFixtureList();
                while(curFixture){
                    UD = static_cast<UserData*>(curFixture->GetUserData());
                    displayData = UD->displayData;
                    if (displayData){
                        b2Shape::Type curFixtureType = curFixture->GetShape()->GetType();
                        if (displayData->isVisible){
                            if (curFixtureType ==  b2Shape::e_polygon){
                                int count = ((b2PolygonShape*)curFixture->GetShape())->GetVertexCount();
                                b2Vec2 points[count];
                                for(int i=0; i < count; i++){
                                    points[i]=((b2PolygonShape*)curFixture->GetShape())->GetVertex(i);
                                }
                                TextureData* TD_p = dynamic_cast<TextureData*>(displayData);
                                KeyLineData* KLD_p = dynamic_cast<KeyLineData*>(displayData);
                                TriangleTextureData* TTD_p = dynamic_cast<TriangleTextureData*>(displayData);
                                if (TTD_p){
                                    if (isTexturesEnabled){
                                        drawTriangle(points, tmp->GetPosition(), GeneralInfo::deductPeriod(tmp->GetAngle()), TTD_p);
                                    }
                                    else
                                        drawPolygon(points, count, tmp->GetPosition(), GeneralInfo::deductPeriod(tmp->GetAngle()), testKeyLineData);
                                }
                                else
                                    if (TD_p){
                                        if (isTexturesEnabled){
                                            drawPolygon(points, count, tmp->GetPosition(), GeneralInfo::deductPeriod(tmp->GetAngle()), TD_p);
                                            TD_p->changeFrame();
                                        }
                                        else
                                            drawPolygon(points, count, tmp->GetPosition(), GeneralInfo::deductPeriod(tmp->GetAngle()), testKeyLineData);
                                    }
                                    else
                                        if (KLD_p)
                                            drawPolygon(points, count, tmp->GetPosition(), GeneralInfo::deductPeriod(tmp->GetAngle()), KLD_p);
                            }

                            else
                                if (curFixtureType == b2Shape::e_circle){
                                    TextureData* TD_p = dynamic_cast<TextureData*>(displayData);
                                    KeyLineData* KLD_p = dynamic_cast<KeyLineData*> (displayData);
                                    float radius = static_cast<b2CircleShape*>(curFixture->GetShape())->m_radius;
                                    if (KLD_p)
                                        drawCircle(radius, tmp->GetPosition(), KLD_p, GeneralInfo::deductPeriod(tmp->GetAngle()));
                                    else
                                        if (TD_p){
                                            drawRectangle(tmp->GetPosition(), radius, radius, GeneralInfo::deductPeriod(tmp->GetAngle()), TD_p);
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
                    }
                    //draw joints
                    bool isDrawingJoints = false;
                    if (isDrawingJoints){
                        b2JointEdge* curJoint = tmp->GetJointList();
                        while(curJoint){
                            drawCircle(0.07, curJoint->joint->GetAnchorA(), new KeyLineData(Color(255,0,0), DisplayData::Layer::JOINT), 0);
                            drawCircle(0.07, curJoint->joint->GetAnchorB(), new KeyLineData(Color(255,0,0), DisplayData::Layer::JOINT), 0);
                            curJoint = curJoint->next;
                        }
                    }
                    //

                    curFixture=curFixture->GetNext();
                }
            }
            tmp=tmp->GetNext();

        }

        bool isDrawingGrid = true;
        if (isDrawingGrid){
            float step = GRID_STEP;
            float width = 0.06;
            for (int i = 0; i < 10; ++i){
                drawRectangle(b2Vec2(i * step,0), width / kx, 1000, 0, new TextureData(textures->getTextureID(Textures::Type::CRATE), DisplayData::Layer::NEAREST));
                drawRectangle(b2Vec2(0,i * step), 1000, width / ky, 0, new TextureData(textures->getTextureID(Textures::Type::CRATE), DisplayData::Layer::NEAREST));
            }
        }
    }
    //update Box2D
if (!isPaused){
    world->Step(1.0/30.0, 8, 3);
    updateGame();
}
}

void GameWidget::mousePressEvent(QMouseEvent *event) {
    Qt::MouseButtons mouseButtons = event->buttons();
    b2Vec2 worldCoord;
    worldCoord.x =  (event->pos().x() / kx - WIDTH / 2 / kx + player->body->GetWorldCenter().x*M2P/2) * 2 * P2M ;
    worldCoord.y = -(event->pos().y() / ky - HEIGHT / 2 / ky - player->body->GetWorldCenter().y*M2P/2) * 2 * P2M;
    qDebug()<<"Click coords: "<<worldCoord.x<<worldCoord.y;
    if (mouseButtons == Qt::LeftButton)
        player->attackState=Player::AS_SWING;
    else
        if (mouseButtons == Qt::RightButton){
            b2Body* box = addRect(worldCoord, 5, 5, true, Textures::Type::CRATE);
            box->GetFixtureList()->SetSensor(true);
            //box->SetType(b2_kinematicBody);
            destroyBodies.push_back(box);
        }
}
void GameWidget::destroyLandscape(){
    // TO SET COLLISION DETECTION IN CONTACT LISTENER
    b2Body* box = destroyBodies.at(0);
    b2Fixture* boxFixture = box->GetFixtureList();
    b2Fixture* landscapeFixture;


    using namespace ClipperLib;
    Clipper clipper;
    clipper.StrictlySimple(true);
    Paths sub, clp, sol;

    Path pathBox;
    b2PolygonShape* shape = static_cast<b2PolygonShape*>(boxFixture->GetShape());
    int vertexCount =  shape->GetVertexCount();
    for (int i = 0; i < vertexCount; ++i){
        b2Vec2 vertex = shape->GetVertex(i) + box->GetPosition();
        pathBox.push_back(IntPoint(vertex.x * CONVERSION_KOEF, vertex.y * CONVERSION_KOEF));
        //  qDebug()<<pathBox.at(i).X<<pathBox.at(i).Y;
    }

    clp.push_back(pathBox);

    for (b2ContactEdge* ce = box->GetContactList(); ce; ce = ce->next){
        b2Contact* c = ce->contact;
        if (c->IsTouching()){
            //TO ADD LANDSCAPE CHECKING
            if (c->GetFixtureA() == boxFixture)
                landscapeFixture = c->GetFixtureB();
            else
                landscapeFixture = c->GetFixtureA();
            //clipping

            Path path;
            b2PolygonShape* shape = static_cast<b2PolygonShape*>(landscapeFixture->GetShape());
            int vertexCount =  shape->GetVertexCount();

            for (int i = 0; i < vertexCount; ++i){
                b2Vec2 vertex = shape->GetVertex(i) + landscapeFixture->GetBody()->GetPosition();
                path.push_back(IntPoint(vertex.x * CONVERSION_KOEF, vertex.y * CONVERSION_KOEF));
            }

            sub.push_back(path);

            b2Body* landscapeBody = landscapeFixture->GetBody();
            landscapeBody->DestroyFixture(landscapeFixture);
            if (!landscapeBody->GetFixtureList())
                world->DestroyBody(landscapeBody);
        }
    }
    clipper.AddPaths(sub, ptSubject, true);
    clipper.AddPaths(clp, ptClip, true);
    clipper.Execute(ctDifference, sol, pftEvenOdd, pftEvenOdd);
    // ??
    CleanPolygon(pathBox);
    // ??
    for (int i = 0; i < sol.size(); ++i){
        vector<Point*> polyline;
        for (int j = 0; j < sol.at(i).size(); ++j){
            polyline.push_back(new Point(sol[i][j].X / CONVERSION_KOEF,sol[i][j].Y / CONVERSION_KOEF));
        }
        b2FixtureDef fixturedef;

        Triangulation::triangulateChain(polyline, fixturedef, new UserData(new TriangleTextureData(textures->getTextureID(
                                                                                                       Textures::Type::GROUND), DisplayData::Layer::LANDSCAPE)),b2Vec2(0, 0), b2_staticBody);

    }

    world->DestroyBody(box);
    destroyBodies.clear();
}

void GameWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Qt::MouseButtons mouseButtons = event->button();
    if (mouseButtons == Qt::LeftButton){
        player->attackState=Player::AS_HIT;

    }
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
    int key = event->key();
    if (key == Qt::Key_Minus){
        kx /= 2;
        ky /= 2;
    }
    if (key == Qt::Key_Plus){
        kx *= 2;
        ky *= 2;
    }
    if (key == Qt::Key_1){
        TEXTURE_SIZE += 20;
        GRID_STEP = TEXTURE_SIZE;
    }
    if (key == Qt::Key_2){
        TEXTURE_SIZE -= 20;
        GRID_STEP = TEXTURE_SIZE;
    }
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
    if (key == Qt::Key_T)
        if (isTexturesEnabled)
            isTexturesEnabled = false;
        else isTexturesEnabled = true;

    if (key == Qt::Key_J){
        b2dJson json;
        json.writeToFile(world, "json/world.json");
        qDebug()<<"saved";
    }
    if (key == Qt::Key_P)
        isPaused? isPaused = false: isPaused = true;
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

b2Body* GameWidget::addRect(b2Vec2 center, float w, float h, bool dyn, Textures::Type type, bool isSensor) {
    return addRect(center.x, center.y, w, h , dyn, type, isSensor);
}

b2Body* GameWidget::addRect(float x, float y, float w, float h, bool dyn, Textures::Type type, bool isSensor) {
    b2BodyDef bodydef;
    bodydef.position.Set(x,y);
    if(dyn)
        bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);
    b2PolygonShape shape;
    shape.SetAsBox(w/2, h/2);
    b2FixtureDef fixturedef;
    fixturedef.isSensor = isSensor;
    fixturedef.shape = &shape;
    fixturedef.density = 3.0;
    fixturedef.filter.groupIndex = 1;

    b2Fixture* bodyFix = body->CreateFixture(&fixturedef);

    DisplayData* bodyDD = (DisplayData*) new TextureData(textures->getTextureID(type), DisplayData::Layer::OBJECT);

    bodyFix->SetUserData((void*) new UserData(bodyDD));
    body->SetUserData((void*) new UserData);
    return body;
}

b2Body* GameWidget::addSpecRect() {
    b2BodyDef bodydef;
    bodydef.position.Set(20, 10);

    bodydef.type=b2_dynamicBody;
    b2Body* body=world->CreateBody(&bodydef);

    b2PolygonShape shape;

    b2Vec2 vertices [5];


    float sizeK = 1;
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

    DisplayData* bodyDD = (DisplayData*) new TextureData(textures->getTextureID(Textures::Type::CRATE), DisplayData::Layer::OBJECT);
    bodyFix->SetUserData((void*) new UserData(bodyDD));
    body->SetUserData((void*) new UserData);
    return body;
}

void GameWidget::drawRectangleFull(b2Vec2 center, float width, float height, float angle, TextureData* textureData){
    width /= 2;
    height /= 2;
    b2Vec2 points[4] {b2Vec2(-width,-height),b2Vec2(width,-height),
                b2Vec2(width,height),b2Vec2(-width,height)};
    drawPolygon(points, 4, center, angle, textureData);
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
    int tileRow = textureData->texture_p->rows - textureData->currentFrameN / textureData->texture_p->columns;
    tileRow--;
    for (int i = 0; i < 4; ++i){

        texPoints[i].x /= textureData->texture_p->columns;
        texPoints[i].y /= textureData->texture_p->rows;
        texPoints[i].x += (float) tileColumn/textureData->texture_p->columns;
        texPoints[i].y += (float) tileRow/textureData->texture_p->rows;
    }

    //drawing texture

    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(center.x * M2P / WIDTH * kx, center.y*M2P/WIDTH * ky, textureData->layer/ (float) DisplayData::Layer::MAX);
    if(textureData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x * M2P / WIDTH * kx,-player->body->GetWorldCenter().y*M2P/WIDTH * ky, 0);

    glRotatef(angle*180.0/M_PI,0,0,1);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, textureData->texture_p->id);

    glBegin(GL_POLYGON);
    for(int i = 0; i < count; i++){
        glTexCoord2f(texPoints[i].x, texPoints[i].y);
        glVertex2f(points[i].x * M2P / WIDTH * kx, points[i].y * M2P / WIDTH * ky);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void GameWidget::drawChain(b2Vec2* points, b2Vec2 center, int count, KeyLineData *keyLineData) {

    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    glPushMatrix();
    glTranslatef(center.x * M2P / WIDTH * kx, center.y*M2P/WIDTH * ky, keyLineData->layer/ (float) DisplayData::Layer::MAX);
    if(keyLineData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x * M2P / WIDTH * kx,-player->body->GetWorldCenter().y*M2P/WIDTH * ky, 0);

    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < count; i++)
        glVertex2f(points[i].x * M2P / WIDTH * kx, points[i].y * M2P / WIDTH * ky);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawTriangle(b2Vec2* points, b2Vec2 center, float angle, TriangleTextureData *triangleTextureData){
    float textureSize = TEXTURE_SIZE; //meters
    //setting texture's points
    b2Vec2 worldPoints[3];
    for (int i = 0; i < 3; i++){
        worldPoints[i] = points[i] + center;
    }
    bool centerPoint[3] = {true, true, true};

    b2Vec2 leftPoint;
    if (worldPoints[0].x < worldPoints[1].x && worldPoints[0].x < worldPoints[2].x){ leftPoint = worldPoints[0]; centerPoint[0] = false;}
    else
        if (worldPoints[1].x < worldPoints[0].x && worldPoints[1].x < worldPoints[2].x){ leftPoint = worldPoints[1]; centerPoint[1] = false;}
        else
            if (worldPoints[2].x < worldPoints[0].x && worldPoints[2].x < worldPoints[1].x){ leftPoint = worldPoints[2]; centerPoint[2] = false;}
    b2Vec2 rightPoint;
    if (worldPoints[0].x > worldPoints[1].x && worldPoints[0].x > worldPoints[2].x){ rightPoint = worldPoints[0]; centerPoint[0] = false;}
    else
        if (worldPoints[1].x > worldPoints[0].x && worldPoints[1].x > worldPoints[2].x){ rightPoint = worldPoints[1]; centerPoint[1] = false;}
        else
            if (worldPoints[2].x > worldPoints[0].x && worldPoints[2].x > worldPoints[1].x){ rightPoint = worldPoints[2]; centerPoint[2] = false;}
    b2Vec2 centerPointX;
    for (int k = 0; k < 3; ++k)
        if (centerPoint[k]) {
            centerPointX = worldPoints[k]; break;
        }
    for (int k = 0; k < 3; ++k)
        centerPoint[k] = true;

    b2Vec2 lowPoint;
    if (worldPoints[0].y < worldPoints[1].y && worldPoints[0].y < worldPoints[2].y){ lowPoint = worldPoints[0]; centerPoint[0] = false;}
    else
        if (worldPoints[1].y < worldPoints[0].y && worldPoints[1].y < worldPoints[2].y){ lowPoint = worldPoints[1]; centerPoint[1] = false;}
        else
            if (worldPoints[2].y < worldPoints[0].y && worldPoints[2].y < worldPoints[1].y){ lowPoint = worldPoints[2]; centerPoint[2] = false;}
    b2Vec2 upPoint;
    if (worldPoints[0].y > worldPoints[1].y && worldPoints[0].y > worldPoints[2].y){ upPoint = worldPoints[0]; centerPoint[0] = false;}
    else
        if (worldPoints[1].y > worldPoints[0].y && worldPoints[1].y > worldPoints[2].y){ upPoint = worldPoints[1]; centerPoint[1] = false;}
        else
            if (worldPoints[2].y > worldPoints[0].y && worldPoints[2].y > worldPoints[1].y){ upPoint = worldPoints[2]; centerPoint[2] = false;}
    b2Vec2 centerPointY;
    for (int k = 0; k < 3; ++k)
        if (centerPoint[k]) {
            centerPointY = worldPoints[k]; break;
        }
    glColor3f(1, 1, 1);
    int x0, x1, y0, y1;
    if (leftPoint.x < 0)
        x0 = (int) (leftPoint.x / textureSize) - 1;
    else
        x0 = (int) (leftPoint.x / textureSize);
    if (rightPoint.x < 0)
        x1 = (int) (rightPoint.x / textureSize);
    else
        x1 = (int) (rightPoint.x / textureSize) + 1;

    if (lowPoint.y < 0)
        y0 = (int) (lowPoint.y / textureSize) - 1;
    else
        y0 = (int) (lowPoint.y / textureSize);
    if (upPoint.y < 0)
        y1 = (int) (upPoint.y / textureSize);
    else
        y1 = (int) (upPoint.y / textureSize) + 1;


    for (int i = x0; i < x1; ++i){
        for (int j = y0; j < y1; ++j){
            float leftBound = i * textureSize, rightBound = leftBound + textureSize;
            float lowBound = j * textureSize, upBound = lowBound + textureSize;

            using namespace ClipperLib;
            Clipper clipper;
            Paths sub, clp, sol;

            //form square
            Path square;
            square.push_back(IntPoint(leftBound * CONVERSION_KOEF, lowBound * CONVERSION_KOEF));
            square.push_back(IntPoint(leftBound * CONVERSION_KOEF, upBound * CONVERSION_KOEF));
            square.push_back(IntPoint(rightBound * CONVERSION_KOEF, upBound * CONVERSION_KOEF));
            square.push_back(IntPoint(rightBound * CONVERSION_KOEF, lowBound * CONVERSION_KOEF));

            sub.push_back(square);
            //
            //form triangle
            Path triangle;
            for (int l = 0; l < 3; l++){
                triangle.push_back(IntPoint(worldPoints[l].x * CONVERSION_KOEF, worldPoints[l].y * CONVERSION_KOEF));
            }
            //
            clp.push_back(triangle);

            clipper.AddPaths(sub, ptSubject, true);
            clipper.AddPaths(clp, ptClip, true);
            clipper.Execute(ctIntersection, sol, pftEvenOdd, pftEvenOdd);
            if (sol.size()){

                std::vector<b2Vec2> curPolygon;
                for (int l = 0; l < sol.at(0).size(); ++l){
                    curPolygon.push_back(b2Vec2(sol.at(0).at(l).X / CONVERSION_KOEF,
                                                sol.at(0).at(l).Y / CONVERSION_KOEF));
                }
                //drawing polygon


                b2Vec2 gridOffset((i) * textureSize, (j) * textureSize);
                glPushMatrix();
                glTranslatef(gridOffset.x * M2P / WIDTH * kx, gridOffset.y * M2P/WIDTH * ky, triangleTextureData->layer/ (float) DisplayData::Layer::MAX);
                if(triangleTextureData->isShifting)
                    glTranslatef(-player->body->GetWorldCenter().x * M2P / WIDTH * kx,-player->body->GetWorldCenter().y*M2P/WIDTH * ky, 0);

                glRotatef(angle*180.0/M_PI,0,0,1);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, triangleTextureData->texture_p->id);
                glBegin(GL_POLYGON);
                for (int t = 0; t < curPolygon.size(); ++t){
                    b2Vec2  texPoints [curPolygon.size()];

                    curPolygon.at(t) -= gridOffset;
                    texPoints[t] = b2Vec2(curPolygon.at(t).x / textureSize, curPolygon.at(t).y / textureSize);

                    glTexCoord2f(texPoints[t].x, texPoints[t].y);

                    glVertex2f(curPolygon[t].x * M2P / WIDTH * kx, curPolygon[t].y * M2P / WIDTH * ky);

                }

                glEnd();
                glDisable(GL_TEXTURE_2D);
                glPopMatrix();
            }
        }

    }

}

void GameWidget::drawPolygon(b2Vec2* points, int count, b2Vec2 center, float angle, KeyLineData *keyLineData) {
    glPushMatrix();
    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    glTranslatef(center.x * M2P / WIDTH * kx, center.y*M2P/WIDTH * ky, keyLineData->layer/ (float) DisplayData::Layer::MAX);
    if(keyLineData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x * M2P / WIDTH * kx,-player->body->GetWorldCenter().y*M2P/WIDTH * ky, 0);
    glRotatef(angle*180.0/M_PI, 0, 0, 1);
    //glBegin(GL_POLYGON);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<count;i++)
        glVertex2f(points[i].x * M2P / WIDTH * kx, points[i].y * M2P / WIDTH * ky);
    glEnd();
    glPopMatrix();
}

void GameWidget::drawCircle(float radius, b2Vec2 center, KeyLineData *keyLineData, float angle){

    glPushMatrix();
    glColor4f(keyLineData->color.red, keyLineData->color.green, keyLineData->color.blue, keyLineData->color.alpha);
    glTranslatef(center.x * M2P / WIDTH * kx, center.y*M2P/WIDTH * ky, keyLineData->layer/ (float) DisplayData::Layer::MAX);
    if(keyLineData->isShifting)
        glTranslatef(-player->body->GetWorldCenter().x * M2P / WIDTH * kx,-player->body->GetWorldCenter().y*M2P/WIDTH * ky, 0);

    glBegin(GL_LINE_LOOP);

    for (int i=0; i < 360; i += 20)
    {
        float rad = i*M_PI/180.0f;
        glVertex2f(cos(rad)*radius*M2P/WIDTH * kx, sin(rad)*radius*M2P/WIDTH * ky);
    }

    glEnd();


    glBegin(GL_LINE_LOOP);
    glVertex2f(cos(angle)*radius*M2P/WIDTH / 2.0f * kx, sin(angle)*radius*M2P/WIDTH / 2.0f * ky);
    glVertex2f(0, 0);
    glEnd();


    glPopMatrix();

}

void GameWidget::drawNumber(float num, float x, float y, float scale, int precise){
    std::stringstream ss;
    ss<<num;
    string s;
    s = ss.str();
    string shortString;
    for (int i = 0; i < precise; ++i){
        if (i < s.size())
            shortString.push_back(s.at(i));
    }
    drawText(shortString, b2Vec2(x, y), scale);
}

vector<GameWidget::Points4> GameWidget::getTextPoints(string s) {
    vector<Points4> retur;
    TextureData* textureData = new TextureData(textures->getTextureID(Textures::Type::FONT), DisplayData::Layer::HUD);
    for(unsigned int i = 0; i < s.size(); i++) {
        b2Vec2 texPoints[4] = {{0.0f, 0.0f},{1.0f, 0.0f},{1.0f, 1.0f},{0.0f, 1.0f}};
        int currentFrameN = 0;
        if(s.at(i) >= 32 && s.at(i) <= 126)
            currentFrameN = s.at(i) - '!' + 1;
        int tileColumn = currentFrameN % textureData->texture_p->columns;
        int tileRow = textureData->texture_p->rows - currentFrameN / textureData->texture_p->columns;
        tileRow--;
        for (int j = 0; j < 4; ++j) {
            texPoints[j].x /= textureData->texture_p->columns;
            texPoints[j].y /= textureData->texture_p->rows;
            texPoints[j].x += (float) tileColumn/textureData->texture_p->columns;
            texPoints[j].y += (float) tileRow/textureData->texture_p->rows;
        }
        Points4 ret;
        ret.point1 = texPoints[0];
        ret.point2 = texPoints[1];
        ret.point3 = texPoints[2];
        ret.point4 = texPoints[3];
        retur.push_back(ret);
    }
    delete textureData;
    return retur;
}

void GameWidget::drawText(string s, b2Vec2 center, float size) {
    float x = center.x;
    float y = center.y;
    float width = 1 * size;
    float height = 2 * size;
    //width /= s.size();
    vector<Points4> text = getTextPoints(s);
    b2Vec2 points[4];
    b2Vec2 straightImage [4] = {{0.0f, 0.0f},{1.0f, 0.0f},{1.0f, 1.0f},{0.0f, 1.0f}};
    TextureData* textureData = new TextureData(textures->getTextureID(Textures::Type::FONT), DisplayData::Layer::HUD);
    for(unsigned int j = 0; j < s.size(); j++) {
        Points4 temp = text.at(j);
        points[0] = temp.point1;
        points[1] = temp.point2;
        points[2] = temp.point3;
        points[3] = temp.point4;

        glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef((x-width/2*s.size()+width*j )*M2P/WIDTH, (y-height/2)*M2P/WIDTH, textureData->layer/ (float) DisplayData::Layer::MAX);
        //if(textureData->isShifting)
        //    glTranslatef(-player->body->GetWorldCenter().x*M2P/WIDTH,-player->body->GetWorldCenter().y*M2P/WIDTH, 0);

        glRotatef(0*180.0/M_PI,0,0,1);

        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, textureData->texture_p->id);

        glBegin(GL_POLYGON);
        for(int i = 0; i < 4; i++){
            glTexCoord2f(points[i].x, points[i].y);
            glVertex2f(straightImage[i].x*M2P/WIDTH*width,straightImage[i].y*M2P/WIDTH*height);
        }
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    delete textureData;
}
