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

    txPlayer = m_engine->newTexture(Resource("textures/animacije.png"));

    asprPlayer = m_engine->newAnimatedSprite(1, 18, txPlayer);
    asprPlayer->setName("Prince");
    asprPlayer->setPosition(QVector3D(400,200,50));
    asprPlayer->setSize(QVector3D(64,64,64));
    asprPlayer->setFrameLength(3);

    orientState = ORIENT_LEFT;
    playerState = FLYING;
    animState = PLAYER_FLYING;
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
    // Orientation state
    if(orientState == ORIENT_LEFT) {
        asprPlayer->setSize(QVector3D(64,64,1.0f));
    }
    else {
        asprPlayer->setSize(QVector3D(-64,64,1.0f));
    }

    // Player movment state
    if( !feetTouching && (playerBody->getVelocity().y() > 10) ) {
        playerState = FLYING;
        onFeetTimer = 0;
    }

    if( !feetTouching && (playerBody->getVelocity().y() < -10) ) {
        playerState = FALLING;
        onFeetTimer = 0;
    }

    if( ((bodyTouchingL && bodyTouchingLU) || (bodyTouchingR && bodyTouchingRU))
        && (playerBody->getRotation().z() > 30 || playerBody->getRotation().z() < -30)
        && (playerBody->getVelocity().y() < 10 &&  playerBody->getVelocity().y() > -10)
      )
    {
        playerState = FELL_DOWN;
        onFeetTimer = 0;
    }

    if( headTouching && (playerBody->getRotation().z() > 30 || playerBody->getRotation().z() < -30) ) {
        playerState = FELL_DOWN;
        onFeetTimer = 0;
    }

    if(feetTouching && (playerBody->getVelocity().x() < 25 || playerBody->getVelocity().x() > -25)) {
        playerState = STANDING;
        onFeetTimer++;
    }

    if(feetTouching && (playerBody->getVelocity().x() > 25 || playerBody->getVelocity().x() < -25)) {
        playerState = RUNNING;
        onFeetTimer++;
    }


    // Player animation state
    if(playerState == RUNNING) {
        animState = PLAYER_RUNNING;
        asprPlayer->setCurrentFrame(2);
        asprPlayer->setLoop(2,2);
    }

    if(playerState == STANDING) {
        animState = PLAYER_STANDING;
        asprPlayer->setCurrentFrame(0);
        asprPlayer->setLoop(0,0);
    }

    if(playerState == FELL_DOWN) {
        animState = PLAYER_SPLAT;
        asprPlayer->setCurrentFrame(10);
        asprPlayer->setLoop(10,10);
    }

    if(playerState == FLYING) {
        animState = PLAYER_FLYING;
        asprPlayer->setCurrentFrame(4);
        asprPlayer->setLoop(4,4);
    }

    if(playerState == FALLING) {
        animState = PLAYER_FALLING;
        asprPlayer->setCurrentFrame(6);
        asprPlayer->setLoop(6,6);
    }

    if((playerState == FALLING || playerState == FLYING || playerState == RUNNING) && (bodyTouchingL || bodyTouchingR)) {
        animState = PLAYER_HIT_MIDDLE;
        asprPlayer->setCurrentFrame(8);
        asprPlayer->setLoop(8,8);
    }

    if((playerState == FALLING || playerState == FLYING || playerState == RUNNING) && (bodyTouchingLU || bodyTouchingRU)) {
        animState = PLAYER_HIT_HEAD;
        asprPlayer->setCurrentFrame(8);
        asprPlayer->setLoop(8,8);
    }

    if((playerState == FALLING || playerState == FLYING || playerState == RUNNING) && (bodyTouchingLD || bodyTouchingRD)) {
        animState = PLAYER_HIT_LEGS;
        asprPlayer->setCurrentFrame(8);
        asprPlayer->setLoop(8,8);
    }



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
        orientState = ORIENT_LEFT;
        return;
    }

    if(Keyboard::keyRIGHT && Keyboard::keyUP) {
        controlsState = CONTROLS_RIGHT_UP;
        orientState = ORIENT_RIGHT;
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
        orientState = ORIENT_LEFT;
        return;
    }

    if(Keyboard::keyRIGHT) {
        controlsState = CONTROLS_RIGHT;
        orientState = ORIENT_RIGHT;
        return;
    }

    controlsState = CONTROLS_NOTHING;
}

void Player::collide(PhysicsBody *with)
{

}

void Player::debugPrintState()
{
    QString str;

    if(playerState == RUNNING)   str = str + "RUNNING ";
    if(playerState == TURNING)   str = str + "TURNING ";
    if(playerState == FALLING)   str = str + "FALLING ";
    if(playerState == FELL_DOWN) str = str + "FELL_DOWN ";
    if(playerState == FLYING)    str = str + "FLYING ";
    if(playerState == STANDING)   str = str + "ON_FEET ";

    if(animState == PLAYER_RUNNING) str = str + "PLAYER_RUNNING ";
    if(animState == PLAYER_FALLING) str = str + "PLAYER_FALLING ";
    if(animState == PLAYER_HIT_HEAD) str = str + "PLAYER_HIT_HEAD ";
    if(animState == PLAYER_HIT_MIDDLE) str = str + "PLAYER_HIT_MIDDLE ";
    if(animState == PLAYER_HIT_LEGS) str = str + "PLAYER_HIT_LEGS ";
    if(animState == PLAYER_SPLAT) str = str + "PLAYER_SPLAT ";
    if(animState == PLAYER_STANDING) str = str + "PLAYER_STANDING ";
    if(animState == PLAYER_FLYING) str = str + "PLAYER_FLYING ";

    qDebug() << str;

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
