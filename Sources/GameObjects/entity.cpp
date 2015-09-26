#include "entity.h"

#include <qdebug.h>

#define M_PI		3.14159265358979323846


Entity::Entity() : GameObject ()
{
    bodyParts = new BodyParts;
    surfaceAngle = 0;

    EntityList::getInstance().list.push_back(this);

    moveState = MS_STAND;
    moveStateVertical = MSV_STAND;
    isOnLadder = false;
    isJumping = false;
    isSitting = false;
    isStanding = false;
    isUsingLeftLeg = true;
    isAscendingLeg = true;
    isRightDirection = true;
    jumpCooldown = 0;
    jumpCooldownMax = 40;
    useCooldown = 0;
    useCooldownMax = 10;
    vehicle = nullptr;
    weapon =nullptr;
}

void Entity::attack(){
   weapon->attack(isRightDirection,attackState,bodyParts);
}

void Entity::constructBody (bool isMirrored, float x, float y, float angle){
    b2World *world = GeneralInfo::getInstance().world;
    Textures *textures = GeneralInfo::getInstance().textures;
    b2Vec2 scale = {2, 2};

    //load body from json file
    std::string errorMsg;
    b2dJson json;
    json.readFromFile("json/man.json", errorMsg);

    b2FixtureDef fixturedef;
    fixturedef.density = 10.0f;
    fixturedef.filter.maskBits = GeneralInfo::CollisionType::BASIC;
    fixturedef.filter.categoryBits = GeneralInfo::CollisionType::BODYPART;

    string mirrorString;
    if (isMirrored){
        mirrorString = "M";
    }
    else{
        mirrorString = "";
    }

    BodyPart* bodyPart;

    //body
    b2Body* bodyChain = json.getBodyByName("Body"+mirrorString);

    bodyPart = new BodyPart(this, BodyPart::Type::BODY);
    b2Body *body =  Triangulation::triangulateChain(Triangulation::chainToPolyline(bodyChain->GetFixtureList(), scale),
                    fixturedef, new UserData(bodyPart, new KeyLineData(Color(0, 255, 255),
                    DisplayData::Layer::PLAYER)), b2Vec2(x, y) + GeneralInfo:: GeneralInfo::mulb2Vec2( bodyChain->GetPosition(), scale));
    bodyPart->body = body;
    bodyParts->setPart(bodyPart);

    this->body = body;

    //head

    bodyChain = json.getBodyByName("Head"+mirrorString);

    bodyPart = new BodyPart(this, BodyPart::Type::HEAD, body);
    body =  Triangulation::triangulateChain(Triangulation::chainToPolyline(bodyChain->GetFixtureList(), scale),
                                            fixturedef, new UserData(bodyPart, new KeyLineData(Color(0, 255, 255),
                                                                                               DisplayData::Layer::PLAYER)),
                                            b2Vec2(x, y) +  GeneralInfo::mulb2Vec2(bodyChain->GetPosition() , scale));
    bodyPart->body = body;
    bodyParts->setPart(bodyPart);
    // add joint
    float maxMotorTorque = body->GetMass() * 300;
    b2RevoluteJoint* jointOld = static_cast<b2RevoluteJoint*>(json.getJointByName("HeadJ"+mirrorString));
    b2RevoluteJointDef RJD;
    RJD.enableMotor = true;
    RJD.enableLimit = true;
    RJD.maxMotorTorque = maxMotorTorque;
    RJD.motorSpeed = 4;
    RevoluteJointInfo RJI;
    RJI.angleDeviation = 0.1;
    RJI.defaultMotorSpeed = 4;
    RJI.motorSpeed = 10;
    RJD.Initialize(bodyParts->body->body, bodyParts->head->body, b2Vec2(x, y) +  GeneralInfo::mulb2Vec2(jointOld->GetAnchorA() , scale));
    RJI.RJ = static_cast<b2RevoluteJoint*>(world->CreateJoint(&RJD));
    if (isMirrored) RJI.RJ->SetLimits(- RJI.RJ->GetUpperLimit(), - RJI.RJ->GetLowerLimit());
    bodyPart->RJI = RJI;
    //

    //symmetric parts
    for (int i = 0; i < 2; ++i){

        DisplayData::Layer layer;
        if (i) layer = DisplayData::Layer::PLAYER_FAR;
        else layer = DisplayData::Layer::PLAYER_NEAR;

        DisplayData* bodyDD;
        if (i)
            bodyDD = (DisplayData*) new KeyLineData(Color(0, 255, 0), layer);
        else
            bodyDD = (DisplayData*) new KeyLineData(Color(255, 255, 0), layer);

        std::string name, nameJoint;
        BodyPart::Type type;
        b2Body *jointBody;

        fixturedef.friction = 1.3;
        for (int j = 0; j < 6; ++j){
            switch( j ){
            case 0:
                name = "Hip"+mirrorString;
                nameJoint = "HipJ"+mirrorString;
                jointBody = bodyParts->body->body;
                type = BodyPart::Type::HIP;
                RJD.upperAngle = M_PI * 0.9;
                RJD.lowerAngle = - M_PI / 6;
                RJD.maxMotorTorque = maxMotorTorque;
                RJI.defaultMotorSpeed = 4;
                RJI.motorSpeed = 4;
                RJI.angleDeviation = 0.1;
                break;
            case 1:
                name = "Shin" + mirrorString;
                nameJoint = "ShinJ" + mirrorString;
                if (i)
                    jointBody = bodyParts->hip2->body;
                else
                    jointBody = bodyParts->hip->body;
                type = BodyPart::Type::SHIN;
                RJD.upperAngle = 0;
                RJD.lowerAngle = - GeneralInfo::D2R(170.0f);
                RJD.maxMotorTorque = maxMotorTorque;
                RJI.defaultMotorSpeed = 4;
                RJI.motorSpeed = 4;
                RJI.angleDeviation = 0.1;
                break;
            case 2:
                name = "Foot"+mirrorString;
                nameJoint = "FootJ"+mirrorString;
                if (i)
                    jointBody = bodyParts->shin2->body;
                else
                    jointBody = bodyParts->shin->body;
                type = BodyPart::Type::FOOT;
                RJD.upperAngle = M_PI / 2;
                RJD.lowerAngle = - M_PI / 4;
                RJD.maxMotorTorque = maxMotorTorque;
                RJI.defaultMotorSpeed = 4;
                RJI.motorSpeed = 4;
                RJI.angleDeviation = 0.1;
                break;
            case 3:
                name = "Shoulder"+mirrorString;
                nameJoint = "ShoulderJ"+mirrorString;
                jointBody = bodyParts->body->body;
                type = BodyPart::Type::SHOULDER;
                RJD.upperAngle = M_PI;
                RJD.lowerAngle = - M_PI / 2;
                RJD.maxMotorTorque = maxMotorTorque ;
                RJI.defaultMotorSpeed = 1;
                RJI.motorSpeed = 1;
                RJI.angleDeviation = 0.2;
                break;
            case 4:
                name = "Forearm"+mirrorString;
                nameJoint = "ForearmJ"+mirrorString;
                if (i)
                    jointBody = bodyParts->shoulder2->body;
                else
                    jointBody = bodyParts->shoulder->body;
                type = BodyPart::Type::FOREARM;
                RJD.upperAngle = M_PI;
                RJD.lowerAngle = 0;
                RJD.maxMotorTorque = maxMotorTorque ;
                RJI.defaultMotorSpeed = 1;
                RJI.motorSpeed = 1;
                RJI.angleDeviation = 0.2;
                break;
            case 5:
                name = "Wrist"+mirrorString;
                nameJoint = "WristJ"+mirrorString;
                if (i)
                    jointBody = bodyParts->forearm2->body;
                else
                    jointBody = bodyParts->forearm->body;
                type = BodyPart::Type::WRIST;
                RJD.upperAngle = 1;
                RJD.lowerAngle = - 1;
                RJD.maxMotorTorque = maxMotorTorque / 10;
                RJI.defaultMotorSpeed = 0.1;
                RJI.motorSpeed = 1;
                RJI.angleDeviation = 0.2;
                break;
            }

            bodyChain = json.getBodyByName(name);

            bodyPart = new BodyPart(this, type);
            body =  Triangulation::triangulateChain(Triangulation::chainToPolyline(bodyChain->GetFixtureList(), scale),
                                                    fixturedef, new UserData(bodyPart, bodyDD), b2Vec2(x, y) +  GeneralInfo::mulb2Vec2(bodyChain->GetPosition() , scale));
            bodyPart->body = body;
            bodyParts->setPart(bodyPart);

            // add joint
            jointOld = static_cast<b2RevoluteJoint*>(json.getJointByName(nameJoint));
            RJD.Initialize(jointBody, bodyPart->body,  GeneralInfo::mulb2Vec2(jointOld->GetAnchorA(), scale) + b2Vec2(x, y));
            RJI.RJ = static_cast<b2RevoluteJoint*>(world->CreateJoint(&RJD));
            if (isMirrored) RJI.RJ->SetLimits(- RJI.RJ->GetUpperLimit(), - RJI.RJ->GetLowerLimit());
            bodyPart->RJI = RJI;
            //
        }
    }


    //body->SetAngularDamping(5);
}

void Entity::rotate(bool right)
{
    if ( (right && !isRightDirection) || (!right && isRightDirection) ){
        if (isRightDirection) isRightDirection = false;
        else isRightDirection = true;
        b2Vec2 pos = bodyParts->body->body->GetPosition();
        delete bodyParts;
        weapon->WJ = nullptr;
        bodyParts = new BodyParts();
        constructBody(!isRightDirection, pos.x, pos.y);

        float weaponAngle;
        if (isRightDirection)
            weaponAngle = - M_PI / 2;
        else weaponAngle = M_PI / 2;
        if(weapon) weapon->transform(bodyParts->wrist2->body,
                                     bodyParts->wrist2->body->GetPosition(), bodyParts->wrist2->body->GetPosition(), weaponAngle);
    }
}

void Entity::move()
{
    float direction;
    if (isRightDirection) direction = 1;
    else direction = -1;

    if (isRightDirection){
        bodyParts->foot->RJI.desiredAngle = surfaceAngle + 0;
        bodyParts->foot2->RJI.desiredAngle = surfaceAngle + 0;
    }
    else{
        bodyParts->foot->RJI.desiredAngle = surfaceAngle + 0;
        bodyParts->foot2->RJI.desiredAngle = surfaceAngle + 0;
    }
    BodyPart *hip, *shin;
    BodyPart *shoulder, *forearm, *foot;
    if (isUsingLeftLeg){
        hip = bodyParts->hip;
        shin = bodyParts->shin;
        foot = bodyParts->foot;
        shoulder = bodyParts->shoulder2;
        forearm = bodyParts->forearm2;
    }
    else{
        hip = bodyParts->hip2;
        shin = bodyParts->shin2;
        foot = bodyParts->foot2;
        shoulder = bodyParts->shoulder;
        forearm = bodyParts->forearm;
    }
    if (isAscendingLeg){
        float hipMaxAngle =  GeneralInfo::D2R(70.0f) + surfaceAngle * 2;
        if (moveStateVertical == MSV_UP)
            hipMaxAngle =  GeneralInfo::D2R(80.0f);
        float shinMaxAngle =  GeneralInfo::D2R(-50.0f) - surfaceAngle;
        if ( (hip->RJI.RJ->GetJointAngle() < hipMaxAngle && isRightDirection ) ||
             (hip->RJI.RJ->GetJointAngle() > - hipMaxAngle && !isRightDirection)){

            hip->RJI.desiredAngle = (hipMaxAngle + GeneralInfo::D2R(10)) * direction;
            hip->RJI.motorSpeed = 2.3;
            shin->RJI.desiredAngle = shinMaxAngle * direction;
            shin->RJI.motorSpeed = 3;
            if (moveStateVertical == MSV_UP)
                    foot->RJI.desiredAngle =  GeneralInfo::D2R(30);
            if(attackState != GeneralInfo::AS_SWING && isUsingLeftLeg)
            {
                shoulder->RJI.desiredAngle =  GeneralInfo::D2R (30.0f * direction);
                shoulder->RJI.motorSpeed = 2;

                forearm->RJI.desiredAngle =  GeneralInfo::D2R (40.0f * direction);
                forearm->RJI.motorSpeed = 2;
            }

        }
        else{
            isAscendingLeg = false;

            if (isUsingLeftLeg) bodyParts->foot->RJI.desiredAngle = 0;
            else bodyParts->foot2->RJI.desiredAngle = 0;

            shoulder->RJI.desiredAngle =  GeneralInfo::D2R (-5.0f * direction);
            forearm->RJI.desiredAngle = 0;

            hip->RJI.desiredAngle =  GeneralInfo::D2R (-15.0f * direction);
            if (moveStateVertical == MSV_UP){
                shin->RJI.desiredAngle =  GeneralInfo::D2R(-20) * direction;
            }
            else
                shin->RJI.desiredAngle =  GeneralInfo::D2R(0) * direction;
            float verticalImpulse = body->GetMass() * (3 + surfaceAngle * 12);
            if (moveStateVertical == MSV_UP){
                body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 5 * direction, verticalImpulse * 2), body->GetPosition() - b2Vec2(0, 3), true);
                float footAngle = 10;
                bodyParts->foot->RJI.desiredAngle =  GeneralInfo::D2R(footAngle);
                bodyParts->foot2->RJI.desiredAngle =  GeneralInfo::D2R(footAngle);
            }
            else
                body->ApplyLinearImpulse(b2Vec2 (body->GetMass() * 5 * direction, verticalImpulse), body->GetPosition() - b2Vec2(0, 1.5), true);

        }

    }
    else{
        //descending leg

        float ascendLimit = 5;

        if (moveStateVertical == MSV_UP)
            ascendLimit =  GeneralInfo::D2R(10);
        BodyPart* curFoot;
        if (isUsingLeftLeg) curFoot = bodyParts->foot;
        else curFoot = bodyParts->foot2;

        // if (fabs(bodyParts->hip->RJI.RJ->GetJointAngle() - bodyParts->hip2->RJI.RJ->GetJointAngle()) > 3.0f)
        if ((hip->RJI.RJ->GetJointAngle() < ascendLimit && isRightDirection) ||
                (hip->RJI.RJ->GetJointAngle() > - ascendLimit && !isRightDirection) ||
                ( isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() >  GeneralInfo::D2R(10.0f)) ||
                ( !isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() <  GeneralInfo::D2R(-10.0f))
                ){
            if (( isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() >  GeneralInfo::D2R(10.0f)) ||
                    ( !isRightDirection && isGrounded(isUsingLeftLeg) && hip->RJI.RJ->GetJointAngle() <  GeneralInfo::D2R(-10.0f)))
                body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * 10), body->GetPosition(), true);
            /*
                BodyPart* foot;
                if (isUsingLeftLeg) foot = bodyParts->foot;
                else foot = bodyParts->foot2;
                if (isRightDirection)
                    surfaceAngle =  foot->RJI.RJ->GetJointAngle() - M_PI / 2;
                else surfaceAngle = M_PI / 2 - foot->RJI.RJ->GetJointAngle() + M_PI;
                qDebug()<<GeneralInfo::R2D(surfaceAngle);
                */
            isAscendingLeg = true;
            changeLeg();
        }
    }
}

void Entity::useObject(){
    if (!useCooldown){
        useCooldown = useCooldownMax;
        if (vehicle) vehicle->left(this);
        else
            if (isOnLadder) isOnLadder = false;
            else
                for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
                {
                    b2Contact* c = ce->contact;
                    UserData* dataA = static_cast<UserData*>(c->GetFixtureA()->GetUserData());
                    if (dataA->gameObject){
                        InteractiveObject* objA = dynamic_cast<InteractiveObject*> (dataA->gameObject);
                        if (objA){
                            objA->use(this);
                            break;
                        }
                    }

                    UserData* dataB = static_cast<UserData*>(c->GetFixtureB()->GetUserData());
                    if (dataB->gameObject){
                        InteractiveObject* objB = dynamic_cast<InteractiveObject*> (dataB->gameObject);
                        if (objB){
                            objB->use(this);
                            break;
                        }
                    }
                }
    }
}

bool Entity::checkForLadder(){
    bool isLadder = false;
    for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
    {
        b2Contact* c = ce->contact;
        UserData* dataA = static_cast<UserData*>(c->GetFixtureA()->GetUserData());
        GameObject* objA = dataA->gameObject;
        Ladder* ladderA = dynamic_cast<Ladder*> (objA);

        UserData* dataB = static_cast<UserData*>(c->GetFixtureB()->GetUserData());
        GameObject* objB = dataB->gameObject;
        Ladder* ladderB = dynamic_cast<Ladder*> (objB);

        if (ladderA || ladderB){
            isLadder = true;
            break;
        }
    }
    return isLadder;
}


void Entity::applyForce(){
    if (vehicle){
        float desiredVel = 0;
        float desiredVelMax = 1 * 360 / 180 * M_PI;
        switch (moveState)
        {
        case MS_LEFT:  desiredVel = desiredVelMax; break;
        case MS_STAND:  desiredVel =  0; break;
        case MS_RIGHT: desiredVel =  - desiredVelMax; break;
        }
        vehicle->motor->SetMotorSpeed(desiredVel);
    }
    else {

        float direction;
        if (isRightDirection) direction = 1;
        else direction = -1;


        BodyPart* foot;
        BodyPart* hip;
        BodyPart* shin;

        BodyPart* forearm;
        BodyPart* shoulder;
        BodyPart* wrist;

        bodyParts->head->RJI.desiredAngle = 0;
        bodyParts->body->RJI.desiredAngle = 0;


        for (int i = 0; i < 2; ++i){
            if (i){
                hip = bodyParts->hip;
                shin = bodyParts->shin;
                shoulder = bodyParts->shoulder;
                forearm = bodyParts->forearm;
                foot = bodyParts->foot;
                wrist = bodyParts->wrist;
            }
            else {
                hip = bodyParts->hip2;
                shin = bodyParts->shin2;
                shoulder = bodyParts->shoulder2;
                forearm = bodyParts->forearm2;
                foot = bodyParts->foot2;
                wrist = bodyParts->wrist2;
            }
            //straight body parts
            foot->RJI.desiredAngle = 0;
            foot->RJI.angleDeviation =  GeneralInfo::D2R(10.0f);
            if (moveState == MoveState::MS_STAND){
                body->SetLinearDamping(5);
                shoulder->RJI.desiredAngle = 0;
                forearm->RJI.desiredAngle = 0;
                shin->RJI.desiredAngle = 0;
                hip->RJI.desiredAngle = 0;
                wrist->RJI.desiredAngle = 0;

                bodyParts->resetSpeed();
            }
            else body->SetLinearDamping(0);
        }


        if (fabs(shin->RJI.desiredAngle - shin->RJI.RJ->GetJointAngle() < shin->RJI.angleDeviation / 2)){
            isStanding = true;
        }
        else isStanding = false;

        //jumping

        if (isJumping)
            jump();


        //moving
        switch ( moveState ){
        case MS_LEFT :
            rotate (false);

            move ();
            break;
        case MS_RIGHT :
            rotate (true);
            move ();
            break;
        case MS_STAND:
            if (GeneralInfo::R2D(GeneralInfo::deductPeriod(shin->body->GetAngle()) - GeneralInfo::deductPeriod(shin->body->GetAngle())) < 10.0f)
                isAscendingLeg = true;
            break;
        }

        // attacking
        switch (attackState) {
        case GeneralInfo::AS_SWING:
            attack();
            break;
        case GeneralInfo::AS_HIT:
            attack();
            break;
        }

        //vertical forces
        if (isOnLadder){
            body->SetLinearVelocity(b2Vec2(0, 0));
            body->ApplyForceToCenter(b2Vec2 (0, 9.82 * body->GetMass()), true);
            if (moveStateVertical == MSV_UP)
                body->ApplyForceToCenter(b2Vec2 (0, 100 * body->GetMass()), true);
            if (moveStateVertical == MSV_DOWN)
                body->ApplyForceToCenter(b2Vec2 (0, - 100 * body->GetMass()), true);
        }
        else{
            if (moveStateVertical == MSV_UP)
                if (checkForLadder()) isOnLadder = true;
            if (moveStateVertical == MSV_DOWN)
                crouch();
        }
    }
    bodyParts->setSpeed();


}


void Entity::changeLeg(){
    if (isUsingLeftLeg)
        isUsingLeftLeg = false;
    else isUsingLeftLeg = true;
}

void Entity::crouch(){
    for (int i = 0; i < 2; ++i){
        BodyPart *shoulder, *forearm, *wrist, *hip, *shin, *foot;
        if (i){
            shoulder = bodyParts->shoulder;
            forearm = bodyParts->forearm;
            wrist = bodyParts->wrist;
            hip = bodyParts->hip;
            shin = bodyParts->shin;
            foot = bodyParts->foot;
        }
        else{
            shoulder = bodyParts->shoulder2;
            forearm = bodyParts->forearm2;
            wrist = bodyParts->wrist2;
            hip = bodyParts->hip2;
            shin = bodyParts->shin2;
            foot = bodyParts->foot2;
        }
        float direction;
        if (isRightDirection)
            direction = 1;
        else direction = -1;

        forearm->RJI.desiredAngle =  GeneralInfo::D2R (90) * direction;
        hip->RJI.desiredAngle =  GeneralInfo::D2R (90) * direction;
        shin->RJI.desiredAngle =  GeneralInfo::D2R (-130) * direction;
        foot->RJI.desiredAngle =  GeneralInfo::D2R(45) * direction;
        if (fabs(fabs(shin->RJI.desiredAngle - shin->RJI.RJ->GetJointAngle() < shin->RJI.angleDeviation / 4) &&
                 fabs(hip->RJI.desiredAngle - hip->RJI.RJ->GetJointAngle() < hip->RJI.angleDeviation / 4))){
            isSitting = true;
        }
        else isSitting = false;
    }
}

void Entity::fall()
{
    body->ApplyTorque(30000, true);
}

bool Entity::isGrounded(){
    for (b2ContactEdge* ce = bodyParts->foot->body->GetContactList(); ce; ce = ce->next)
    {
        if (ce->contact->IsTouching())
            return true;
    }
    for (b2ContactEdge* ce = bodyParts->foot2->body->GetContactList(); ce; ce = ce->next)
    {
        if (ce->contact->IsTouching())
            return true;
    }
    return false;
}

bool Entity::isGrounded(bool leftLeg){
    if (leftLeg)
        for (b2ContactEdge* ce = bodyParts->foot->body->GetContactList(); ce; ce = ce->next)
        {
            if (ce->contact->IsTouching())
                return true;
        }
    else
        for (b2ContactEdge* ce = bodyParts->foot2->body->GetContactList(); ce; ce = ce->next)
        {
            if (ce->contact->IsTouching())
                return true;
        }
    return false;
}

void Entity::jump(){
    if (isGrounded())
        if (!isSitting)
            crouch();
        else{
            for (int i = 0; i < 2; ++i){
                BodyPart *shoulder, *forearm, *wrist, *hip, *shin, *foot;
                if (i){
                    hip = bodyParts->hip;
                    shin = bodyParts->shin;
                    shoulder = bodyParts->shoulder;
                    forearm = bodyParts->forearm;
                    foot = bodyParts->foot;
                    wrist = bodyParts->wrist;
                }
                else {
                    hip = bodyParts->hip2;
                    shin = bodyParts->shin2;
                    shoulder = bodyParts->shoulder2;
                    forearm = bodyParts->forearm2;
                    foot = bodyParts->foot2;
                    wrist = bodyParts->wrist2;
                }
                float jumpK = 2;
                hip->RJI.motorSpeed *= jumpK;
                shin->RJI.motorSpeed *= jumpK;
                foot->RJI.motorSpeed *= jumpK;

            }
            if (isStanding){
                isSitting = false;
                float angle = GeneralInfo::deductPeriod(body->GetAngle());
                float impulse = body->GetMass() * 100;
                body->ApplyLinearImpulse(b2Vec2(impulse * sin(angle), impulse * cos(angle)), body->GetWorldCenter(), true);
            }
        }
    /*
    float jumpHeight;

    jumpHeight = 30;

    isOnLadder = false;
    if (!jumpCooldown){
        jumpCooldown = jumpCooldownMax;
        if (isGrounded())
            body->ApplyLinearImpulse(b2Vec2(0, body->GetMass() * jumpHeight), body->GetPosition(), true);
    }
            */

}

void Entity::update()
{

    //rotate body
    float maxDeviation = 0.005;
    float maxDeviationSpeed = 20;
    if (moveStateVertical == MSV_UP){
        maxDeviation *= 2;
        maxDeviationSpeed /= 3;
    }
    float deviationSpeed = fabs( maxDeviationSpeed  * GeneralInfo::deductPeriod(body->GetAngle()) );
    if (!vehicle){
        if (GeneralInfo::deductPeriod(body->GetAngle()) > maxDeviation)
            body->SetAngularVelocity (-deviationSpeed);
        else
            if (GeneralInfo::deductPeriod(body->GetAngle()) < - maxDeviation)
                body->SetAngularVelocity (deviationSpeed);
        // if (body->GeneralInfo::deductPeriod(GetAngle()) > maxDeviation * 2.0f)
        //     body->SetAngularVelocity(body->GetAngularVelocity() / 2.0f);
    }

    if (!isJumping) isSitting = false;
    if (jumpCooldown) --jumpCooldown;
    if (useCooldown) --useCooldown;
    if (isOnLadder && !checkForLadder()) isOnLadder = false;
    //if (isRightDirection) static_cast<TextureData*>(bodyParts->head->DD)->isMirrored = false;
    //else static_cast<TextureData*>(bodyParts->head->DD)->isMirrored = true;
    applyForce();
}

