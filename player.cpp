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
    asPlayer->transform->setSize(128,128,0);
    asPlayer->setFrameLength(3);

    //stateQueue.enqueue(PLAYER_STANDING, 0);
    //stateQueue.setInterface(this);
    orientState = PLAYER_LEFT;
    playerState = FLYING;
    turnTimer = 0;
    isFeetTouching = false;
//    isFlying = true;

//    move_x = 0;
//    moving = false;
//    didJump = false;

    m_engine->physicsWorld->setDamping(0.9f);


    playerBody = m_engine->createPhysicsObjectCircle(80,60);
    playerBody->setPosition(900,300);
    //playerBody->setFriction(20.0f);
    //playerBody->setMaxVelocity(150);
    m_engine->addPhysicsObject(playerBody);
    playerBody->parentGameObject = this;
    playerBody->userType = GAME_PLAYER;

    feetSensor = m_engine->createPhysicsObjectBox(1,110,15, PHYSICSBODY_ROUGE);
    feetSensor->setPosition(900,215);
}

void Player::checkState()
{
    if(orientState == PLAYER_LEFT) {
        asPlayer->transform->setSize(128,128,0);
    }
    else {
        asPlayer->transform->setSize(-128,128,0);
    }



    switch (playerState) {
    case STANDING:
        if(controlsState == CONTROLS_LEFT && orientState == PLAYER_RIGHT) {
            playerState = TURNING;
            return;
        }
        if(controlsState == CONTROLS_RIGHT && orientState == PLAYER_LEFT) {
            playerState = TURNING;
            return;
        }
        if(controlsState == CONTROLS_RIGHT || controlsState == CONTROLS_LEFT) {
            playerState = RUNNING;
            return;
        }
        if(controlsState == CONTROLS_UP) {
            playerState = JUMP_STANDIG;
            return;
        }
        if(controlsState == CONTROLS_RIGHT_UP || controlsState == CONTROLS_LEFT_UP ) {
            playerState = JUMP_RUNNING;
            return;
        }

        break;
    case RUNNING:
        if(controlsState == CONTROLS_LEFT && orientState == PLAYER_RIGHT) {
            playerState = TURNING;
            return;
        }
        if(controlsState == CONTROLS_RIGHT && orientState == PLAYER_LEFT) {
            playerState = TURNING;
            return;
        }
        if(controlsState == CONTROLS_UP || controlsState == CONTROLS_RIGHT_UP || controlsState == CONTROLS_LEFT_UP) {
            playerState = JUMP_RUNNING;
            return;
        }
        if(controlsState == CONTROLS_NOTHING) {
            playerState = STANDING;
            return;
        }
        break;
    case TURNING:
        turnTimer++;
        if(turnTimer > 10) {
            if(controlsState == CONTROLS_NOTHING) {
                playerState = STANDING;
            }
            if(controlsState == CONTROLS_LEFT || controlsState == CONTROLS_RIGHT) {
                playerState = RUNNING;
            }
            if(controlsState == CONTROLS_LEFT_UP || controlsState == CONTROLS_RIGHT_UP) {
                playerState = JUMP_RUNNING;
            }
            if(controlsState == CONTROLS_UP) {
                playerState = JUMP_STANDIG;
            }
            turnTimer = 0;

            if(orientState == PLAYER_LEFT)
                orientState = PLAYER_RIGHT;
            else
                orientState = PLAYER_LEFT;

            return;
        }
        break;
    case FLYING:
        if(isFeetTouching) {
            if(controlsState == CONTROLS_NOTHING) {
                playerState = STANDING;
            }
            if(controlsState == CONTROLS_LEFT || controlsState == CONTROLS_RIGHT) {
                playerState = RUNNING;
            }
            return;
        }
        break;
    case JUMP_RUNNING:
        if(!isFeetTouching) {
            playerState = FLYING;
            return;
        }
        break;
    case JUMP_STANDIG:
        if(!isFeetTouching) {
            playerState = FLYING;
            return;
        }
        break;
    default:
        break;
    }




    /*
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
            stateQueue.enqueue(PLAYER_START_JUMP_FROM_RUN, 14);
            stateQueue.enqueue(PLAYER_FLYING, 0);
        }
        if(stateQueue.currentState() == PLAYER_BEGIN_RUNNING) {
            stateQueue.removeAll();
            stateQueue.enqueue(PLAYER_START_JUMP_FROM_RUN, 14);
            stateQueue.enqueue(PLAYER_FLYING, 0);
        }
    }
    */
}

void Player::onStateEntered(int state)
{
    /*
    switch (state) {
    case PLAYER_BEGIN_RUNNING:
        asPlayer->setLoop(32,36);
        moving = true;
        break;
    case PLAYER_STANDING:
        asPlayer->setCurrentFrame(0);
        asPlayer->setLoop(0,0);
        moving = false;
        break;
    case PLAYER_RUNNING:
        asPlayer->setLoop(10,17);
        moving = true;
        break;
    case PLAYER_END_BEGIN_RUNNING:
        asPlayer->setLoop(36,32);
        moving = false;
        break;
    case PLAYER_END_RUNNING:
        asPlayer->setLoop(19,26);
        moving = false;
        break;
    case PLAYER_TURN:
        asPlayer->setLoop(0,8);
        moving = false;
        break;
    case PLAYER_TURN_FROM_RUN:
        asPlayer->setLoop(50,58);
        moving = false;
        break;
    case PLAYER_START_JUMP_FROM_RUN:
        asPlayer->setLoop(38,42);
        moving = true;
        break;
    case PLAYER_END_JUMP_FROM_RUN:
        asPlayer->setLoop(44,48);
        moving = true;
        break;
    case PLAYER_FLYING:
        asPlayer->setLoop(44,44);
        moving = true;
        didJump = true;
        break;
    default:
        break;
    }
    */
}

void Player::onStateExited(int state)
{
    /*
    switch (state) {
    case PLAYER_TURN_FROM_RUN:
    case PLAYER_TURN:
        if(orientState == PLAYER_LEFT) {
            orientState = PLAYER_RIGHT;
            asPlayer->transform->setSize(-128,128,0);
        }
        else {
            orientState = PLAYER_LEFT;
            asPlayer->transform->setSize(128,128,0);
        }
        break;
    default:
        break;
    }
    */
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
        return;
    }

    if(Keyboard::keyRIGHT && Keyboard::keyUP) {
        controlsState = CONTROLS_RIGHT_UP;
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
    debugPrintState();

    asPlayer->transform->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y(), 0.0f);
    feetSensor->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y()-75);

    // Check is flying
    QList<PhysicsObject*> ret = m_engine->checkForOverlappingPhysicsObjects(feetSensor);
    if(ret.size() > 0) {
        isFeetTouching = true;
    }
    else {
        isFeetTouching = false;
    }

//    switch (controlsState) {
//    case CONTROLS_LEFT:
//        playerBody->applyImpulse(-500,0);
//        break;
//    case CONTROLS_RIGHT:
//        playerBody->applyImpulse(500,0);
//        break;
//    case CONTROLS_LEFT_UP:
//        playerBody->applyImpulse(-500,1500);
//        break;
//    case CONTROLS_RIGHT_UP:
//        playerBody->applyImpulse(500,1500);
//        break;
//    case CONTROLS_UP:
//        playerBody->applyImpulse(0,1500);
//        break;
//    default:
//        break;
//    }


    //if(playerState != FLYING) playerBody->setMaxVelocity(150);
    //else playerBody->setMaxVelocity(5500);


    if(playerState != FLYING) {
        if(playerState == JUMP_RUNNING) {
            if(orientState == PLAYER_LEFT)
                playerBody->applyImpulse(-400,1600);
            else
                playerBody->applyImpulse(400,1600);
        }

        if(playerState == JUMP_STANDIG) {
                playerBody->applyImpulse(0,1600);
        }

        if(playerState == RUNNING) {
            if(orientState == PLAYER_LEFT) playerBody->applyImpulse(-500,0);
            else playerBody->applyImpulse(500,0);
        }

        if(playerState == STANDING || playerState == TURNING) {
            if(playerBody->getVelocity().x() < 58 && playerBody->getVelocity().x() > -58) {
                playerBody->setVelocity(0,playerBody->body->v.y);
            }
            else {
                if(orientState == PLAYER_LEFT) playerBody->applyImpulse(500,0);
                else playerBody->applyImpulse(-500,0);
            }
        }

        if(playerBody->getVelocity().x() > 150 ) playerBody->setVelocity(150, playerBody->getVelocity().y());
        if(playerBody->getVelocity().x() < -150 ) playerBody->setVelocity(-150, playerBody->getVelocity().y());
    }



    /*
    feetSensor->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y()-75);
    QList<PhysicsObject*> ret = m_engine->checkForOverlappingPhysicsObjects(feetSensor);

    if(ret.size() > 0 && stateQueue.currentState() == PLAYER_FLYING) {
        isFlying = false;
        stateQueue.removeAll();
        stateQueue.enqueue(PLAYER_END_JUMP_FROM_RUN, 14);
        stateQueue.enqueue(PLAYER_RUNNING, 0);
    }
    else {
        isFlying = true;
    }

    stateQueue.update(dt);
    checkKey();
    checkState();

    asPlayer->transform->setPosition(playerBody->getPosition().x(), playerBody->getPosition().y(), 0.0f);
    //asPlayer->transform->setRotation(0,0,playerBody->getRotation());

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

             qDebug() << "END FLZZZ";
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

    if(didJump) {
        playerBody->applyImpulse(0,18500);
        didJump = false;
    }
    else {

    }
    */
}
