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

    txPlayer = m_engine->newTexture(Resource("textures/animacije.png"));

    asprPlayer = m_engine->newAnimatedSprite(1, 18, txPlayer);
    asprPlayer->setName("Prince");
    asprPlayer->setPosition(QVector3D(400,200,50));
    asprPlayer->setSize(QVector3D(64,64,64));
    asprPlayer->setFrameLength(5);

    orientState = ORIENT_LEFT;
    playerState = FLYING;
    animState = PLAYER_FLYING;
    onFeetTimer = 0;
    jumpAllowed = true;

    //Settings::DEBUG_PHYSICS_RENDER = true;

    m_engine->physicsWorld->setDamping(0.9f);

    playerBody = m_engine->newPhysicsBodyBox(80,24,48);
    playerBody->setPosition(QVector3D(100,600,1));
    playerBody->parentGameObject = this;
    playerBody->name = "PlayerBody";
    playerBody->setRotation(QVector3D(0,0,0));

    playerShape = m_engine->newPhysicsShapeBox(24,48,QVector2D(0,0));
    playerShape->name = "PlayerShape";
    playerShape->setFriction(0.5f);
    playerShape->userType = GAME_PLAYER;
    m_engine->addShapeToBody(playerShape, playerBody);
    m_engine->addShapeToSpace(playerShape);

    feetSensorL = m_engine->newPhysicsShapeBox(12,10,QVector2D(-8,-36));
    feetSensorL->setSensor(true);
    feetSensorL->name = "FeetSensorL";
    m_engine->addShapeToBody(feetSensorL, playerBody);
    m_engine->addShapeToSpace(feetSensorL);

    feetSensorR = m_engine->newPhysicsShapeBox(12,10,QVector2D(8,-36));
    feetSensorR->setSensor(true);
    feetSensorR->name = "FeetSensorR";
    m_engine->addShapeToBody(feetSensorR, playerBody);
    m_engine->addShapeToSpace(feetSensorR);

    headSensorL = m_engine->newPhysicsShapeBox(12,10,QVector2D(-8,36));
    headSensorL->setSensor(true);
    headSensorL->name = "HeadSensorL";
    m_engine->addShapeToBody(headSensorL, playerBody);
    m_engine->addShapeToSpace(headSensorL);

    headSensorR = m_engine->newPhysicsShapeBox(12,10,QVector2D(8,36));
    headSensorR->setSensor(true);
    headSensorR->name = "HeadSensorL";
    m_engine->addShapeToBody(headSensorR, playerBody);
    m_engine->addShapeToSpace(headSensorR);

    bodySensorL = m_engine->newPhysicsShapeBox(10,12,QVector2D(-25,0));
    bodySensorL->setSensor(true);
    bodySensorL->name = "HeadSensorL";
    bodySensorL->debug_draw_color = QColor(255,0,0);
    m_engine->addShapeToBody(bodySensorL, playerBody);
    m_engine->addShapeToSpace(bodySensorL);

    bodySensorR = m_engine->newPhysicsShapeBox(10,12,QVector2D(25,0));
    bodySensorR->setSensor(true);
    bodySensorR->name = "HeadSensorR";
    m_engine->addShapeToBody(bodySensorR, playerBody);
    m_engine->addShapeToSpace(bodySensorR);

    //playerBody->setRotation(QVector3D(0,0,180));
    playerBody->disableRotation();
}

void Player::checkState()
{
    // Orientation state
    if(orientState == ORIENT_LEFT) {
        asprPlayer->setSize(QVector3D(-64,64,1.0f));
    }
    else {
        asprPlayer->setSize(QVector3D(64,64,1.0f));
    }

    if( (feetTouchingL && feetTouchingR) ){
        if(controlsState == CONTROLS_LEFT || controlsState == CONTROLS_RIGHT) {
            if(playerBody->getVelocity().x() > 5 || playerBody->getVelocity().x() < -5) {
                playerState = RUNNING;
            }
        }
        else {
            playerState = STANDING;
        }
        onFeetTimer++;
    }
    else {
        if(playerBody->getVelocity().y() > 10) {
            playerState = FLYING;
        }
        else if(playerBody->getVelocity().y() < -10) {
            playerState = FALLING;
        }
        onFeetTimer = 0;
    }

    // Player animation state
    if(playerState == RUNNING) {
        if(animState != PLAYER_RUNNING) {
            asprPlayer->setLoop(2,7);
            animState = PLAYER_RUNNING;
        }
    }

    if(playerState == STANDING) {
        animState = PLAYER_STANDING;
        asprPlayer->setLoop(0,0);
    }

    if(playerState == FLYING) {
        animState = PLAYER_FLYING;
        asprPlayer->setLoop(11,11);
    }

    // Bio je 10
    if(playerState == FALLING) {
        animState = PLAYER_FALLING;
        asprPlayer->setLoop(10,10);
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
    if(playerState == FALLING)   str = str + "FALLING ";
    if(playerState == FLYING)    str = str + "FLYING ";
    if(playerState == STANDING)   str = str + "ON_FEET ";

    if(animState == PLAYER_RUNNING) str = str + "PLAYER_RUNNING ";
    if(animState == PLAYER_FALLING) str = str + "PLAYER_FALLING ";
    if(animState == PLAYER_STANDING) str = str + "PLAYER_STANDING ";
    if(animState == PLAYER_FLYING) str = str + "PLAYER_FLYING ";

    qDebug() << str;
}

void Player::update(float dt)
{
    asprPlayer->setPosition(playerBody->getPosition());
    asprPlayer->setRotation(playerBody->getRotation());

    // Check sensors for collisions
    feetTouchingL = feetSensorL->isOverlapping();
    feetTouchingR = feetSensorR->isOverlapping();
    headTouchingL = headSensorL->isOverlapping();
    headTouchingR = headSensorR->isOverlapping();
    bodyTouchingL = bodySensorL->isOverlapping();
    bodyTouchingR = bodySensorR->isOverlapping();

    checkKey();
    checkState();
    //debugPrintState();

    if(controlsState == CONTROLS_LEFT || controlsState == CONTROLS_LEFT_UP) {
            if(playerBody->getVelocity().x() > 2) { playerBody->applyImpulse(-700,0); }
            else { playerBody->applyImpulse(-400,0); }
    }

    if(controlsState == CONTROLS_RIGHT || controlsState == CONTROLS_RIGHT_UP) {
        if(playerBody->getVelocity().x() < -2) { playerBody->applyImpulse(700,0); }
        else { playerBody->applyImpulse(400,0); }
    }

    if(controlsState != CONTROLS_UP && controlsState != CONTROLS_LEFT_UP && controlsState != CONTROLS_RIGHT_UP) {
        if(onFeetTimer > 4) { jumpAllowed = true; }
    }

    if(controlsState == CONTROLS_UP || controlsState == CONTROLS_LEFT_UP || controlsState == CONTROLS_RIGHT_UP) {
        if(jumpAllowed) {
            jumpAllowed = false;
            playerBody->applyImpulse(0,17400);
        }
    }

    if(controlsState == CONTROLS_NOTHING) {
        if(feetTouchingL && feetTouchingR) {
            if(playerBody->getVelocity().x() > 2) { playerBody->applyImpulse(-600,0); }
            if(playerBody->getVelocity().x() < -2) { playerBody->applyImpulse(600,0); }
        }
    }

    // Gurnuti ga sa ivice ako ne stoji sa obije noge
    if((feetTouchingL && !feetTouchingR) && playerBody->getVelocity().y() == 0) {
        playerBody->applyImpulse(200,0);
    }
    if((!feetTouchingL && feetTouchingR) && playerBody->getVelocity().y() == 0) {
        playerBody->applyImpulse(-200,0);
    }

    // limit Horizontalni na 150
    if(playerBody->getVelocity().x() > 150 ) { playerBody->setVelocity(150, playerBody->getVelocity().y()); }
    if(playerBody->getVelocity().x() < -150 ) { playerBody->setVelocity(-150, playerBody->getVelocity().y()); }

}
