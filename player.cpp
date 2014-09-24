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
    m_engine = engine;
    m_engine->addGameObject(this);

    txPlayer = m_engine->newTexture(Resource("textures/allframes.png"));

    planet = m_engine->newTexture(Resource("textures/Planet.png"));
    sprPlanet = m_engine->newSprite(planet);
    sprPlanet->setName("Planet");
    sprPlanet->setPosition(QVector3D(100,1,10));
    sprPlanet->setSize(QVector3D(128.0f,128.0f,128.0f));
    sprPlanet->setRotation(QVector3D(0.0f,0.0f,40.0f));

    asPlayer = m_engine->newAnimatedSprite(4, 32, txPlayer);
    asPlayer->setName("Prince");
    asPlayer->setPosition(QVector3D(1,1,50));
    asPlayer->setSize(QVector3D(64,64,1.0f));
    asPlayer->setFrameLength(3);
    asPlayer->addChild(sprPlanet);

    //stateQueue.enqueue(PLAYER_STANDING, 0);
    //stateQueue.setInterface(this);
    orientState = PLAYER_LEFT;
    playerState = FLYING;
    turnTimer = 0;
    fellDownTimer = 0;
    onFeetTimer = 0;
    jumpAllowed = true;

    m_engine->physicsWorld->setDamping(0.9f);

    playerBody = m_engine->createPhysicsBodyBox(80,32,64);
    //playerBody->setFriction(0.5f);
    playerBody->setPosition(QVector3D(100,600,1));
    //m_engine->addPhysicsObject(playerBody);
    playerBody->parentGameObject = this;
    //playerBody->userType = GAME_PLAYER;
    playerBody->name = "PlayerBody";
    playerBody->setRotation(QVector3D(0,0,0));

    playerShape = m_engine->createPhysicsShapeBox(playerBody,100,100,QVector2D());
    //playerShape->setFriction(0.5f);
/*
    feetSensor = m_engine->createPhysicsBodyBox(1,50,50, PHYSICS_BODY_ROUGE);
    feetSensor->name = "FeetSensor";
    feetSensor->setPosition(QVector3D(119,100,1));
    feetSensor->setRotation(QVector3D(0.0,0.0,45.0));
    //playerBody->addChild(feetSensor);
    */

/*
    headSensorL = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    headSensorL->setPosition(QVector3D(800,210,0));
    headSensorLU = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    headSensorLU->setPosition(QVector3D(800,210,0));

    headSensorR = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    headSensorR->setPosition(QVector3D(1000,210,0));
    headSensorRU = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    headSensorRU->setPosition(QVector3D(1000,210,0));


    headSensorT = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    headSensorT->setPosition(QVector3D(900,410,0));
    */
}

void Player::checkState()
{
    if(orientState == PLAYER_LEFT) {
        asPlayer->setSize(QVector3D(64,64,1.0f));
    }
    else {
        asPlayer->setSize(QVector3D(-64,64,1.0f));
    }

    if(!feetTouching && (playerBody->getVelocity().y() > 2 || playerBody->getVelocity().y() < -2)) {
        playerState = FLYING;
        onFeetTimer = 0;
    }

    if((playerBody->getRotation().x() > 30 || playerBody->getRotation().x() < -30)) {
        playerState = FALLING;
        onFeetTimer = 0;
    }

    if( ( (headTouchingL && headTouchingLU) || (headTouchingR && headTouchingRU) || headTouchingT)
        && (playerBody->getRotation().x() > 30 || playerBody->getRotation().x() < -30)
      )
    {
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
    if(playerState == ON_FEET) qDebug() << "ON_FEET" << controlsState;
    if(playerState == RUNNING) qDebug() << "RUNNING" << controlsState;
    if(playerState == TURNING) qDebug() << "TURNING" << controlsState;
    if(playerState == FALLING) qDebug() << "FALLING" << controlsState;
    if(playerState == FELL_DOWN) qDebug() << "FELL_DOWN" << controlsState;
    if(playerState == FLYING) qDebug() << "FLYING" << controlsState;
}

void Player::update(float dt)
{
    //sprPlanet->setRotation(QVector3D(0,0,40));

    asPlayer->setPosition(playerBody->getPosition()); //  QVector3D(playerBody->getPosition().x(), playerBody->getPosition().y(), 0.0f));
    asPlayer->setRotation(playerBody->getRotation());// QVector3D(0,0,playerBody->getRotation()));
/*
    feetSensor->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y()-38);
    QVector2D fr = rotateAround(feetSensor->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    feetSensor->setPosition(fr.x(), fr.y());

    headSensorL->setPosition(playerBody->getPosition().x()-24, playerBody->getPosition().y()-20);
    QVector2D hsl = rotateAround(headSensorL->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    headSensorL->setPosition(hsl.x(), hsl.y());
    headSensorLU->setPosition(playerBody->getPosition().x()-24, playerBody->getPosition().y()+20);
    QVector2D hslu = rotateAround(headSensorLU->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    headSensorLU->setPosition(hslu.x(), hslu.y());

    headSensorR->setPosition(playerBody->getPosition().x()+24, playerBody->getPosition().y()-20);
    QVector2D hsr = rotateAround(headSensorR->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    headSensorR->setPosition(hsr.x(), hsr.y());
    headSensorRU->setPosition(playerBody->getPosition().x()+24, playerBody->getPosition().y()+20);
    QVector2D hsru = rotateAround(headSensorRU->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    headSensorRU->setPosition(hsru.x(), hsru.y());


    headSensorT->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y()+38);
    QVector2D hst = rotateAround(headSensorT->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    headSensorT->setPosition(hst.x(), hst.y());
*/
    // Feet sensor check
    //feetTouching = feetSensor->isOverlapping();
/*
    // Head L check
    headTouchingL = headSensorL->isOverlapping();

    // Head LU check
    headTouchingLU = headSensorLU->isOverlapping();

    // Head R check
    headTouchingR = headSensorR->isOverlapping();

    // Head RU check
    headTouchingRU = headSensorRU->isOverlapping();

    // Head T check
    headTouchingT = headSensorT->isOverlapping();

    // Body check
    bodyTouching = playerBody->isOverlapping();
*/

    checkKey();
    //checkState();
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

    if(controlsState == CONTROLS_NOTHING || playerState == FALLING || playerState == FELL_DOWN)
    {
        if(bodyTouching) {
            //if(playerBody->getVelocity().x() > 0) playerBody->setVelocity(playerBody->getVelocity().x()-5, playerBody->getVelocity().y());
            //if(playerBody->getVelocity().x() < 0) playerBody->setVelocity(playerBody->getVelocity().x()+5, playerBody->getVelocity().y());
            //if(playerBody->getVelocity().x() < 8 && playerBody->getVelocity().x() > -8) playerBody->setVelocity(0,playerBody->body->v.y);
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
