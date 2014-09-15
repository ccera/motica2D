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

    txPlayer.setFile(Resource("textures/allframes.png"));
    m_engine->addTexture(&txPlayer);

    asPlayer.setTexture(&txPlayer);
    asPlayer.setName("Prince");
    asPlayer.transform->setPosition(1000,90,60);
    asPlayer.transform->setSize(128,128,0);
    asPlayer.setRows(4);
    asPlayer.setColumns(32);
    asPlayer.setFrameLength(3);
    m_engine->addAnimatedSprite(&asPlayer);

    stateQueue.enqueue(PLAYER_STANDING, 0);
    stateQueue.setInterface(this);
    orientState = PLAYER_LEFT;
    move_x = 0;
    moving = false;
    isFlying = true;

    playerBody = m_engine->physicsWorld->createCircle(80,60); //m_engine->physicsWorld->createBox(80.0f, 128.0f, 128.0f);
    playerBody->setPosition(900,100);
    //playerBody->setFriction(20.0f);
    playerBody->setMaxVelocity(150);
    m_engine->addPhysicsObject(playerBody);
    playerBody->parentGameObject = this;
    playerBody->userType = GAME_PLAYER;
}

void Player::checkState()
{
    if(controlsState == CONTROLS_LEFT)
    {
        if(stateQueue.currentState() == PLAYER_STANDING && orientState == PLAYER_LEFT) {
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_BEGIN_RUNNING, 14);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
        if(stateQueue.currentState() == PLAYER_STANDING && orientState == PLAYER_RIGHT) {
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0);
        }
        if(stateQueue.currentState() == PLAYER_BEGIN_RUNNING && orientState == PLAYER_RIGHT) {
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_END_BEGIN_RUNNING, 14);
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0); //??
        }
        if(stateQueue.currentState() == PLAYER_RUNNING && orientState == PLAYER_RIGHT) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_TURN_FROM_RUN, 26);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
        if(stateQueue.currentState() == PLAYER_END_RUNNING && orientState == PLAYER_RIGHT) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_TURN_FROM_RUN, 26);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
    }
    else if(controlsState == CONTROLS_RIGHT)
    {
        if(stateQueue.currentState() == PLAYER_STANDING && orientState == PLAYER_RIGHT) {
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_BEGIN_RUNNING, 14);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
        if(stateQueue.currentState() == PLAYER_STANDING && orientState == PLAYER_LEFT) {
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0);
        }
        if(stateQueue.currentState() == PLAYER_BEGIN_RUNNING && orientState == PLAYER_LEFT) {
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_END_BEGIN_RUNNING, 14);
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0); //??
        }
        if(stateQueue.currentState() == PLAYER_RUNNING && orientState == PLAYER_LEFT) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_TURN_FROM_RUN, 26);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
        if(stateQueue.currentState() == PLAYER_END_RUNNING && orientState == PLAYER_LEFT) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_TURN_FROM_RUN, 26);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
    }
    else if(controlsState == CONTROLS_NOTHING)
    {
        if(stateQueue.currentState() == PLAYER_RUNNING) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_END_RUNNING, 23);
            stateQueue.enqueue(PLAYER_STANDING, 0);
        }
        if(stateQueue.currentState() == PLAYER_BEGIN_RUNNING) {
            stateQueue.removeAllExceptCurrent();
            stateQueue.enqueue(PLAYER_END_BEGIN_RUNNING, 14);
            stateQueue.enqueue(PLAYER_STANDING, 0);
        }
    }
    else if(controlsState == CONTROLS_UP)
    {
        if(stateQueue.currentState() == PLAYER_RUNNING) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_JUMP_FROM_RUN, 29);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
        if(stateQueue.currentState() == PLAYER_BEGIN_RUNNING) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_JUMP_FROM_RUN, 29);
            stateQueue.enqueue(PLAYER_RUNNING, 0);
        }
    }
}

void Player::onStateEntered(int state)
{
    switch (state) {
    case PLAYER_BEGIN_RUNNING:
        asPlayer.setLoop(32,36);
        moving = true;
        jumping = false;
        break;
    case PLAYER_STANDING:
        asPlayer.setCurrentFrame(0);
        asPlayer.setLoop(0,0);
        moving = false;
        jumping = false;
        isFlying = false;
        break;
    case PLAYER_RUNNING:
        asPlayer.setLoop(10,17);
        moving = true;
        jumping = false;
        break;
    case PLAYER_END_BEGIN_RUNNING:
        asPlayer.setLoop(36,32);
        moving = false;
        jumping = false;
        break;
    case PLAYER_END_RUNNING:
        asPlayer.setLoop(19,26);
        moving = false;
        jumping = false;
        break;
    case PLAYER_TURN:
        asPlayer.setLoop(0,8);
        moving = false;
        jumping = false;
        break;
    case PLAYER_TURN_FROM_RUN:
        asPlayer.setLoop(50,58);
        moving = false;
        jumping = false;
        break;
    case PLAYER_JUMP_FROM_RUN:
        asPlayer.setLoop(38,48);
        moving = true;
        jumping = true;
        break;
    default:
        break;
    }
}

void Player::onStateExited(int state)
{
    switch (state) {
    case PLAYER_TURN_FROM_RUN:
    case PLAYER_TURN:
        if(orientState == PLAYER_LEFT) {
            orientState = PLAYER_RIGHT;
            asPlayer.transform->setSize(-128,128,0);
        }
        else {
            orientState = PLAYER_LEFT;
            asPlayer.transform->setSize(128,128,0);
        }
        break;
    default:
        break;
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
        controlsState = CONTROLS_UP;
        return;
    }

    if(Keyboard::keyRIGHT && Keyboard::keyUP) {
        controlsState = CONTROLS_UP;
        return;
    }

    if(Keyboard::keyUP) {
        controlsState = CONTROLS_UP;
        return;
    }

    if(Keyboard::keyLEFT) {
        controlsState = CONTROLS_LEFT;
        return;
    }

    if(Keyboard::keyRIGHT) {
        controlsState = CONTROLS_RIGHT;
        return;
    }

    controlsState = CONTROLS_NOTHING;
}

void Player::collide(PhysicsObject *with)
{
    /*
    //qDebug() << "colidee";
    if(with->userType == GAME_FLOOR) {
        isFlying = false;
    }
    else {
        isFlying = true;
        qDebug() << "colidee";
    }
    */
}

void Player::update(float dt)
{
    QList<PhysicsObject*> ret = m_engine->physicsWorld->checkForOverlappingObjects(playerBody);

    if(ret.size() > 0) {
        isFlying = false;
    }
    else {
        isFlying = true;
    }

    stateQueue.update(dt);
    checkKey();
    checkState();

    asPlayer.transform->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y(), 0.0f);
    //asPlayer.transform->setRotation(0,0,playerBody->getRotation());

    if(moving) {
        if(orientState == PLAYER_LEFT)
            playerBody->applyImpulse(-500,0);
        else
            playerBody->applyImpulse(500,0);
    }
    else {

        if(!isFlying) {
        if(playerBody->getVelocity().x() < 8 && playerBody->getVelocity().x() > -8)
            playerBody->setVelocity(0,0);


        if(orientState == PLAYER_LEFT) {
            if(playerBody->getVelocity().x() < -10.0)
                playerBody->applyImpulse(600,0);
        }
        else {
            if(playerBody->getVelocity().x() > 10.0) {
                playerBody->applyImpulse(-600,0);
            }
        }
        }
    }

    if(jumping && !isFlying) {
        playerBody->applyImpulse(0,18500);
        isFlying = true;
    }
    else {

    }

/*
    if(moving) {
        move_x += 0.3;
        if(move_x > 5.0) move_x = 5.0;
    }
    else {
        move_x -= 0.3;
        if(move_x <= 0.0) move_x = 0.0;
    }

    if(jumping) {
        move_y += 0.3;
        if(move_y > 5.0) move_y = 5.0;
    }
    else {
        move_y -= 0.3;
        if(move_y <= 0.0) move_y = 0.0;
    }

/*

    float mx = 0.0;
    if(orientState == PLAYER_LEFT) {
        mx = move_x * -1;
    }
    else {
        mx = move_x;
    }

    asPlayer.transform->translateFor(mx,0,0);
    */
}
