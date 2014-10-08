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
    asprPlayer->setFrameLength(10);

    orientState = ORIENT_LEFT;
    playerState = FLYING;
    animState = PLAYER_FLYING;
    turnTimer = 0;
    fellDownTimer = 0;
    onFeetTimer = 0;
    jumpAllowed = true;
    bodySensorsTouching = 0;

    //Settings::DEBUG_PHYSICS_RENDER = true;

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

    feetSensorL = m_engine->newPhysicsShapeBox(23,10,QVector2D(-12,-40));
    feetSensorL->setSensor(true);
    feetSensorL->name = "FeetSensorL";
    m_engine->addShapeToBody(feetSensorL, playerBody);
    m_engine->addShapeToSpace(feetSensorL);

    feetSensorR = m_engine->newPhysicsShapeBox(23,10,QVector2D(12,-40));
    feetSensorR->setSensor(true);
    feetSensorR->name = "FeetSensorR";
    m_engine->addShapeToBody(feetSensorR, playerBody);
    m_engine->addShapeToSpace(feetSensorR);

    headSensorL = m_engine->newPhysicsShapeBox(23,10,QVector2D(-12,40));
    headSensorL->setSensor(true);
    headSensorL->name = "HeadSensorL";
    m_engine->addShapeToBody(headSensorL, playerBody);
    m_engine->addShapeToSpace(headSensorL);

    headSensorR = m_engine->newPhysicsShapeBox(23,10,QVector2D(12,40));
    headSensorR->setSensor(true);
    headSensorR->name = "HeadSensorL";
    m_engine->addShapeToBody(headSensorR, playerBody);
    m_engine->addShapeToSpace(headSensorR);

    bodySensorL = m_engine->newPhysicsShapeBox(10,15,QVector2D(-25,0));
    bodySensorL->setSensor(true);
    bodySensorL->name = "HeadSensorL";
    bodySensorL->debug_draw_color = QColor(255,0,0);
    m_engine->addShapeToBody(bodySensorL, playerBody);
    m_engine->addShapeToSpace(bodySensorL);

    bodySensorR = m_engine->newPhysicsShapeBox(10,15,QVector2D(25,0));
    bodySensorR->setSensor(true);
    bodySensorR->name = "HeadSensorR";
    m_engine->addShapeToBody(bodySensorR, playerBody);
    m_engine->addShapeToSpace(bodySensorR);

    //playerBody->setRotation(QVector3D(0,0,180));
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
            if(playerBody->getVelocity().x() > 10 || playerBody->getVelocity().x() < -10) {
            playerState = RUNNING;
            onFeetTimer++;
            }
        }
        else {
            playerState = STANDING;
            onFeetTimer++;
        }
    }
    else {
        if(playerBody->getVelocity().y() > 10) {
            playerState = FLYING;
            onFeetTimer = 0;
        }
        else if(playerBody->getVelocity().y() < -10) {
            playerState = FALLING;
            onFeetTimer = 0;
        }
    }

    if( (playerBody->getRotation().z() > 55 || playerBody->getRotation().z() < -55) )
    {
        if( (feetTouchingL && bodyTouchingL)  || (feetTouchingR && bodyTouchingR) )
        {
            playerState = FELL_DOWN;
            onFeetTimer = 0;
            jumpAllowed = false;
        }

        if( (headTouchingL && bodyTouchingL)  || (headTouchingR && bodyTouchingR) )
        {
            playerState = FELL_DOWN;
            onFeetTimer = 0;
            jumpAllowed = false;
        }

        if( (headTouchingL && headTouchingR) ){
            playerState = FELL_DOWN;
            onFeetTimer = 0;
            jumpAllowed = false;
        }
    }

    // Player animation state
    if(playerState == RUNNING) {
        if(animState != PLAYER_RUNNING) {
            asprPlayer->setCurrentFrame(2);
            asprPlayer->setLoop(2,7);
            animState = PLAYER_RUNNING;
        }
    }

    if(playerState == STANDING) {
        animState = PLAYER_STANDING;
        asprPlayer->setCurrentFrame(0);
        asprPlayer->setLoop(0,0);
    }

    if(playerState == FELL_DOWN) {
        animState = PLAYER_SPLAT;
        if(orientState == ORIENT_LEFT) {
            if(bodyTouchingL) {
                asprPlayer->setCurrentFrame(15);
                asprPlayer->setLoop(15,15);
            }
            else {
                asprPlayer->setCurrentFrame(14);
                asprPlayer->setLoop(14,14);
            }
        }

        if(orientState == ORIENT_RIGHT) {
            if(bodyTouchingL) {
                asprPlayer->setCurrentFrame(14);
                asprPlayer->setLoop(14,14);
            }
            else {
                asprPlayer->setCurrentFrame(15);
                asprPlayer->setLoop(15,15);
            }
        }

        if(headTouchingL && headTouchingR) {
            asprPlayer->setCurrentFrame(17);
            asprPlayer->setLoop(17,17);
        }
    }

    if(playerState == FLYING) {
        animState = PLAYER_FLYING;
        asprPlayer->setCurrentFrame(11);
        asprPlayer->setLoop(11,11);
    }

    // Bio je 10
    if(playerState == FALLING) {
        animState = PLAYER_FALLING;
        asprPlayer->setCurrentFrame(10);
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
        if(playerState != FELL_DOWN) { orientState = ORIENT_LEFT; }
        return;
    }

    if(Keyboard::keyRIGHT && Keyboard::keyUP) {
        controlsState = CONTROLS_RIGHT_UP;
        if(playerState != FELL_DOWN) { orientState = ORIENT_RIGHT; }
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
        if(playerState != FELL_DOWN) { orientState = ORIENT_LEFT; }
        return;
    }

    if(Keyboard::keyRIGHT) {
        controlsState = CONTROLS_RIGHT;
        if(playerState != FELL_DOWN) { orientState = ORIENT_RIGHT; }
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
}

void Player::update(float dt)
{
    asprPlayer->update(dt);
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

    if(playerState == FELL_DOWN) {
        fellDownTimer++;
        if(fellDownTimer > 130) {
            playerBody->setRotation(QVector3D(0,0,0));
            fellDownTimer = 0;
        }
    }

    if(controlsState == CONTROLS_LEFT || controlsState == CONTROLS_LEFT_UP) {
        if(playerState != FALLING && playerState != FELL_DOWN) {
            playerBody->applyImpulse(-300,0);
        }
    }

    if(controlsState == CONTROLS_RIGHT || controlsState == CONTROLS_RIGHT_UP) {
        if(playerState != FALLING && playerState != FELL_DOWN) {
            playerBody->applyImpulse(300,0);
        }
    }

    if(controlsState != CONTROLS_UP && controlsState != CONTROLS_LEFT_UP && controlsState != CONTROLS_RIGHT_UP) {
        if(onFeetTimer > 1) {
            jumpAllowed = true;
        }
    }

    if(controlsState == CONTROLS_UP || controlsState == CONTROLS_LEFT_UP || controlsState == CONTROLS_RIGHT_UP) {
        if(jumpAllowed) {
            jumpAllowed = false;
            playerBody->applyImpulse(0,16400);
        }
    }

    // limit Horizontalni na 150
    if(playerBody->getVelocity().x() > 150 ) playerBody->setVelocity(150, playerBody->getVelocity().y());
    if(playerBody->getVelocity().x() < -150 ) playerBody->setVelocity(-150, playerBody->getVelocity().y());

}
