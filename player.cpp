//
//  Copyright (c) 2014 Emir Cerić, Dejan Omasta. All rights reserved.
//
//  This file is part of Motica2D.
//
//  Motica2D is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Motica2D is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Motica2D.  If not, see <http://www.gnu.org/licenses/>.
//

#include "player.h"

Player::Player(Engine *engine) :
     GameObject()
{
    t = 0;
    m_engine = engine;
    m_engine->addGameObject(this);

    txPlayer = m_engine->newTexture(Resource("textures/allframes.png"));

    planet = m_engine->newTexture(Resource("textures/yellow.png"));
    sprPlanet = m_engine->newSprite(planet);
    sprPlanet->setName("Planet");
    sprPlanet->setPosition(QVector3D(500,200,50));
    sprPlanet->setSize(QVector3D(28.0f, 28.0f, 28.0f));

    asprPlayer = m_engine->newAnimatedSprite(4, 32, txPlayer);
    asprPlayer->setName("Prince");
    asprPlayer->setPosition(QVector3D(400,200,50));
    asprPlayer->setSize(QVector3D(64,64,64));
    asprPlayer->setFrameLength(3);
    asprPlayer->addChild(sprPlanet);

    //stateQueue.enqueue(PLAYER_STANDING, 0);
    //stateQueue.setInterface(this);
    orientState = PLAYER_LEFT;
    playerState = FLYING;
    turnTimer = 0;
    fellDownTimer = 0;
    onFeetTimer = 0;
    jumpAllowed = true;

    m_engine->physicsWorld->setDamping(0.9f);

    playerBody = m_engine->newPhysicsBodyBox(80,32,64);
    playerBody->setPosition(QVector3D(100,600,1));
    playerBody->parentGameObject = this;
    playerBody->name = "PlayerBody";
    playerBody->setRotation(QVector3D(0,0,0));

    playerShape = m_engine->newPhysicsShapeBox(32,64,QVector2D(0,0));
    playerShape->name = "PlayerShape";
    playerShape->setFriction(0.5f);
    playerShape->userType = GAME_PLAYER;
    m_engine->addShapeToBody(playerShape, playerBody);
    m_engine->addShapeToSpace(playerShape);

    feetSensor = m_engine->newPhysicsShapeBox(10,10,QVector2D(0,-40));
    feetSensor->setSensor(true);
    feetSensor->name = "FeetSensor";
    m_engine->addShapeToBody(feetSensor, playerBody);
    m_engine->addShapeToSpace(feetSensor);

    headSensor = m_engine->newPhysicsShapeBox(10,10,QVector2D(0,40));
    headSensor->setSensor(true);
    headSensor->name = "HeadSensor";
    m_engine->addShapeToBody(headSensor, playerBody);
    m_engine->addShapeToSpace(headSensor);

    bodySensorL = m_engine->newPhysicsShapeBox(10,10,QVector2D(-25,0));
    bodySensorL->setSensor(true);
    bodySensorL->name = "HeadSensorL";
    m_engine->addShapeToBody(bodySensorL, playerBody);
    m_engine->addShapeToSpace(bodySensorL);

    bodySensorLU = m_engine->newPhysicsShapeBox(10,10,QVector2D(-25,20));
    bodySensorLU->setSensor(true);
    bodySensorLU->name = "HeadSensorLU";
    m_engine->addShapeToBody(bodySensorLU, playerBody);
    m_engine->addShapeToSpace(bodySensorLU);

    bodySensorLD = m_engine->newPhysicsShapeBox(10,10,QVector2D(-25,-20));
    bodySensorLD->setSensor(true);
    bodySensorLD->name = "HeadSensorLD";
    m_engine->addShapeToBody(bodySensorLD, playerBody);
    m_engine->addShapeToSpace(bodySensorLD);

    bodySensorR = m_engine->newPhysicsShapeBox(10,10,QVector2D(25,0));
    bodySensorR->setSensor(true);
    bodySensorR->name = "HeadSensorR";
    m_engine->addShapeToBody(bodySensorR, playerBody);
    m_engine->addShapeToSpace(bodySensorR);

    bodySensorRU = m_engine->newPhysicsShapeBox(10,10,QVector2D(25,20));
    bodySensorRU->setSensor(true);
    bodySensorRU->name = "HeadSensorRU";
    m_engine->addShapeToBody(bodySensorRU, playerBody);
    m_engine->addShapeToSpace(bodySensorRU);

    bodySensorRD = m_engine->newPhysicsShapeBox(10,10,QVector2D(25,-20));
    bodySensorRD->setSensor(true);
    bodySensorRD->name = "HeadSensorRD";
    m_engine->addShapeToBody(bodySensorRD, playerBody);
    m_engine->addShapeToSpace(bodySensorRD);
}

void Player::checkState()
{
    if(orientState == PLAYER_LEFT) {
        asprPlayer->setSize(QVector3D(64,64,1.0f));
    }
    else {
        asprPlayer->setSize(QVector3D(-64,64,1.0f));
    }

    if( !feetTouching && (playerBody->getVelocity().y() > 10) ) {
        playerState = FLYING;
        onFeetTimer = 0;
    }

    if( !feetTouching && (playerBody->getVelocity().y() < -10) ) {
        playerState = FALLING;
        onFeetTimer = 0;
    }

    if( ((bodyTouchingL && bodyTouchingLU) || (bodyTouchingR && bodyTouchingRU))
        && (playerBody->getRotation().z() > 30 || playerBody->getRotation().z() < -30) ) {
        playerState = FELL_DOWN;
        onFeetTimer = 0;
    }

    if( headTouching && (playerBody->getRotation().z() > 30 || playerBody->getRotation().z() < -30) ) {
        playerState = FELL_DOWN;
        onFeetTimer = 0;
    }

    if(feetTouching) {
        playerState = ON_FEET;
        onFeetTimer++;
    }

    //qDebug() << playerBody->getRotation();

    /*
            stateQueue.enqueue(PLAYER_BEGIN_RUNNING, 14);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0);
            stateQueue.enqueue(PLAYER_END_BEGIN_RUNNING, 14);
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0); //??
            stateQueue.enqueue(PLAYER_TURN_FROM_RUN, 26);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
            stateQueue.enqueue(PLAYER_TURN_FROM_RUN, 26);
    /*
    case PLAYER_BEGIN_RUNNING:
        asPlayer->setLoop(32,36);
    case PLAYER_STANDING:
        asPlayer->setCurrentFrame(0);
        asPlayer->setLoop(0,0);
    case PLAYER_RUNNING:
        asPlayer->setLoop(10,17);
    case PLAYER_END_BEGIN_RUNNING:
        asPlayer->setLoop(36,32);
    case PLAYER_END_RUNNING:
        asPlayer->setLoop(19,26);
    case PLAYER_TURN:
        asPlayer->setLoop(0,8);
    case PLAYER_TURN_FROM_RUN:
        asPlayer->setLoop(50,58);
    case PLAYER_START_JUMP_FROM_RUN:
        asPlayer->setLoop(38,42);
    case PLAYER_END_JUMP_FROM_RUN:
        asPlayer->setLoop(44,48);
    case PLAYER_FLYING:
        asPlayer->setLoop(44,44);
    }
    */
}

void Player::onStateEntered(int state)
{

}

void Player::onStateExited(int state)
{

}

void Player::checkKey()
{
    if(Keyboard::keyLEFT && Keyboard::keyRIGHT && Keyboard::keyUP && Keyboard::keyDOWN) {
        controlsState = CONTROLS_NOTHING;
        return;
    }

    if(Keyboard::keyLEFT && Keyboard::keyRIGHT) {
        controlsState = CONTROLS_NOTHING;
        return;
    }

    if(Keyboard::keyLEFT && Keyboard::keyUP) {
        controlsState = CONTROLS_LEFT_UP;
        orientState = PLAYER_LEFT;
        return;
    }

    if(Keyboard::keyRIGHT && Keyboard::keyUP) {
        controlsState = CONTROLS_RIGHT_UP;
        orientState = PLAYER_RIGHT;
        return;
    }

    if(Keyboard::keyUP) {
        controlsState = CONTROLS_UP;
        return;
    }

    if(Keyboard::keyDOWN) {
        controlsState = CONTROLS_DOWN;
        return;
    }

    if(Keyboard::keyLEFT) {
        controlsState = CONTROLS_LEFT;
        orientState = PLAYER_LEFT;
        return;
    }

    if(Keyboard::keyRIGHT) {
        controlsState = CONTROLS_RIGHT;
        orientState = PLAYER_RIGHT;
        return;
    }

    controlsState = CONTROLS_NOTHING;
}

void Player::collide(PhysicsBody *with)
{

}

void Player::debugPrintState()
{
    //if(playerState == ON_FEET)      qDebug() << "ON_FEET" << controlsState;
    if(playerState == RUNNING)      qDebug() << "RUNNING" << controlsState;
    if(playerState == TURNING)      qDebug() << "TURNING" << controlsState;
    if(playerState == FALLING)      qDebug() << "FALLING" << controlsState;
    if(playerState == FELL_DOWN)    qDebug() << "FELL_DOWN" << controlsState;
    if(playerState == FLYING)       qDebug() << "FLYING" << controlsState;

    /*
        qDebug() << "  feet " << feetTouching
                 << "  head " << headTouching
                 << "  bodyL " << bodyTouchingL
                 << "  bodyLU " << bodyTouchingLU
                 << "  bodyLD " << bodyTouchingLD
                 << "  bodyR " << bodyTouchingR
                 << "  bodyRU " << bodyTouchingRU
                 << "  bodyRD " << bodyTouchingRD  ;

        qDebug() << playerBody->getVelocity().y() << " rot" << playerBody->getRotation();
    */
}

void Player::update(float dt)
{
    asprPlayer->setPosition(playerBody->getPosition());
    asprPlayer->setRotation(playerBody->getRotation());

    // Check sensors for collisions
    feetTouching = feetSensor->isOverlapping();
    headTouching = headSensor->isOverlapping();
    bodyTouchingL = bodySensorL->isOverlapping();
    bodyTouchingLU = bodySensorLU->isOverlapping();
    bodyTouchingLD = bodySensorLD->isOverlapping();
    bodyTouchingR = bodySensorR->isOverlapping();
    bodyTouchingRU = bodySensorRU->isOverlapping();
    bodyTouchingRD = bodySensorRD->isOverlapping();

    checkKey();
    checkState();
    //debugPrintState();

    if(playerState == FELL_DOWN) {
        fellDownTimer++;
        if(fellDownTimer > 130) {
            playerBody->setRotation(QVector3D(0,0,0));
            fellDownTimer = 0;
        }
    }

    if(playerBody->getVelocity().y() > 3) {
        feetTouching = false;
    }

    if(controlsState == CONTROLS_LEFT || controlsState == CONTROLS_LEFT_UP) {
        playerBody->applyImpulse(-300,0);
    }

    if(controlsState == CONTROLS_RIGHT || controlsState == CONTROLS_RIGHT_UP) {
        playerBody->applyImpulse(300,0);
    }

    if(controlsState != CONTROLS_UP && controlsState != CONTROLS_LEFT_UP && controlsState != CONTROLS_RIGHT_UP) {
        if(onFeetTimer > 1) {
            jumpAllowed = true;
        }
    }

    if(controlsState == CONTROLS_UP || controlsState == CONTROLS_LEFT_UP || controlsState == CONTROLS_RIGHT_UP) {
        if(feetTouching && jumpAllowed) {
            jumpAllowed = false;
            playerBody->applyImpulse(0,16400);
        }
    }

    // limit Horizontalni na 150
    if(playerBody->getVelocity().x() > 150 ) playerBody->setVelocity(150, playerBody->getVelocity().y());
    if(playerBody->getVelocity().x() < -150 ) playerBody->setVelocity(-150, playerBody->getVelocity().y());

}

QVector2D Player::rotateAround(const QVector2D &pos, const QVector2D &around, float angle)
{
    QVector3D p(pos.x(), pos.y(), 0.0f);
    QVector3D a(around.x(), around.y(), 0.0f);
    QMatrix4x4 m;

    p-=a;
    m.rotate(0, 1.0f, 0.0f, 0.0f);
    m.rotate(0, 0.0f, 1.0f, 0.0f);
    m.rotate(-angle, 0.0f, 0.0f, 1.0f);
    p = p * m;
    p+=a;

    return QVector2D(p.x(), p.y());
}
