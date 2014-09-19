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

    asPlayer = m_engine->newAnimatedSprite(4, 32, txPlayer);
    asPlayer->setName("Prince");
    asPlayer->transform->setPosition(1000,90,60);
    asPlayer->transform->setSize(64,64,0);
    asPlayer->setFrameLength(3);

    //stateQueue.enqueue(PLAYER_STANDING, 0);
    //stateQueue.setInterface(this);
    orientState = PLAYER_LEFT;
    playerState = FLYING;
    turnTimer = 0;
    feetSensor = false;
    headSensorL = false;
    headSensorR = false;

    m_engine->physicsWorld->setDamping(0.9f);

    playerBody = m_engine->createPhysicsObjectBox(80,32,64);
    //playerBody->disableRotation();
    playerBody->setPosition(900,300);
    m_engine->addPhysicsObject(playerBody);
    playerBody->parentGameObject = this;
    playerBody->userType = GAME_PLAYER;

    feetSensor = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    feetSensor->setPosition(900,210);

    headSensorL = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    headSensorL->setPosition(800,210);

    headSensorR = m_engine->createPhysicsObjectBox(1,8,4, PHYSICSBODY_ROUGE);
    headSensorR->setPosition(1000,210);
}

void Player::checkState()
{
    if(orientState == PLAYER_LEFT) {
        asPlayer->transform->setSize(64,64,0);
    }
    else {
        asPlayer->transform->setSize(-64,64,0);
    }

    if(!feetSensor) {
        playerState = FLYING;
    }

    if(feetSensor && (playerBody->getRotation() > 45 || playerBody->getRotation() < -45)){
            qDebug() << "FELLLLLLL DOWNNNNN";
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

void Player::collide(PhysicsObject *with)
{

}

void Player::debugPrintState()
{
    if(playerState == STANDING) qDebug() << "STANDING" << controlsState;
    if(playerState == RUNNING) qDebug() << "RUNNING" << controlsState;
    if(playerState == TURNING) qDebug() << "TURNING" << controlsState;
    if(playerState == JUMP_STANDIG) qDebug() << "JUMP_STANDIG" << controlsState;
    if(playerState == JUMP_RUNNING) qDebug() << "JUMP_RUNNING" << controlsState;
    if(playerState == FLYING) qDebug() << "FLYING" << controlsState;
}

void Player::update(float dt)
{
    checkKey();
    checkState();
    //debugPrintState();

    asPlayer->transform->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y(), 0.0f);
    asPlayer->transform->setRotation(0,0,playerBody->getRotation());
    feetSensor->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y()-38);
    headSensorL->setPosition(playerBody->getPosition().x()-24, playerBody->getPosition().y()+10);
    headSensorR->setPosition(playerBody->getPosition().x()+24, playerBody->getPosition().y()+10);

    QVector2D fr = rotateAround(feetSensor->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    feetSensor->setPosition(fr.x(), fr.y());
    QVector2D hsl = rotateAround(headSensorL->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    headSensorL->setPosition(hsl.x(), hsl.y());
    QVector2D hsr = rotateAround(headSensorR->getPosition(), playerBody->getPosition(), playerBody->getRotation());
    headSensorR->setPosition(hsr.x(), hsr.y());

    // Feet sensor check
    QList<PhysicsObject*> ret = m_engine->checkForOverlappingPhysicsObjects(feetSensor);
    if(ret.size() > 0) { feetSensor = true; }
    else { feetSensor = false; }

    // Head L check
    QList<PhysicsObject*> ret2 = m_engine->checkForOverlappingPhysicsObjects(headSensorL);
    if(ret2.size() > 0) { headSensorL = true; }
    else { headSensorL = false; }

    // Head R check
    QList<PhysicsObject*> ret3 = m_engine->checkForOverlappingPhysicsObjects(headSensorLR;
    if(ret3.size() > 0) { headSensorR = true; }
    else { headSensorR = false; }


    if(playerBody->getVelocity().y() > 3) {
        feetSensor = false;
    }


    if(controlsState == CONTROLS_LEFT || controlsState == CONTROLS_LEFT_UP) {
        playerBody->applyImpulse(-200,0);
    }

    if(controlsState == CONTROLS_RIGHT || controlsState == CONTROLS_RIGHT_UP) {
        playerBody->applyImpulse(200,0);
    }

    if(controlsState == CONTROLS_UP || controlsState == CONTROLS_LEFT_UP || controlsState == CONTROLS_RIGHT_UP) {
        if(flyTimer < 8 && feetSensor)
            playerBody->applyImpulse(0,16400);
    }

    if(controlsState == CONTROLS_DOWN) {
        //if(flyTimer < 8)
        //qDebug() << "jj";
        playerBody->applyImpulse(0,-400);
    }

    if(controlsState == CONTROLS_NOTHING) {
        if(headSensorL) {
        if(playerBody->getVelocity().x() > 0) playerBody->setVelocity(playerBody->getVelocity().x()-5, playerBody->getVelocity().y());
        if(playerBody->getVelocity().x() < 0) playerBody->setVelocity(playerBody->getVelocity().x()+5, playerBody->getVelocity().y());
        if(playerBody->getVelocity().x() < 8 && playerBody->getVelocity().x() > -8) playerBody->setVelocity(0,playerBody->body->v.y);
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
