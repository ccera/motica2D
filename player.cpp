#include "player.h"

Player::Player(MoEngine *engine) :
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
    asPlayer.setNumOfFrames(55);
    asPlayer.setFrameLength(3);
    m_engine->addAnimatedSprite(&asPlayer);

    stateQueue.enqueue(PLAYER_STANDING, 0);
    stateQueue.setInterface(this);
    orientState = PLAYER_LEFT;
    move_x = 0;
    moving = false;
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
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_END_RUNNING, 23);
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0); //??
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
            stateQueue.dequeue();
            stateQueue.enqueue(PLAYER_END_RUNNING, 23);
            stateQueue.enqueue(PLAYER_TURN, 26);
            stateQueue.enqueue(PLAYER_STANDING, 0); //??
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
}

void Player::onStateEntered(int state)
{
    switch (state) {
    case PLAYER_BEGIN_RUNNING:
        asPlayer.setLoop(50,54);
        moving = true;
        break;
    case PLAYER_STANDING:
        asPlayer.currentFrame = 0;
        asPlayer.setLoop(0,0);
        moving = false;
        break;
    case PLAYER_RUNNING:
        asPlayer.setLoop(10,17);
        moving = true;
        break;
    case PLAYER_END_BEGIN_RUNNING:
        asPlayer.setLoop(54,50);
        moving = false;
        break;
    case PLAYER_END_RUNNING:
        asPlayer.setLoop(19,26);
        moving = false;
        break;
    case PLAYER_TURN:
        asPlayer.setLoop(0,8);
        moving = false;
        break;
    default:
        break;
    }
}

void Player::onStateExited(int state)
{
    switch (state) {
    case PLAYER_TURN:
        if(orientState == PLAYER_LEFT) {
            orientState = PLAYER_RIGHT;
            asPlayer.horizontalMirror = true;
        }
        else {
            orientState = PLAYER_LEFT;
            asPlayer.horizontalMirror = false;
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

void Player::update(float dt)
{
    stateQueue.update(dt);
    checkKey();
    checkState();

    if(moving) {
        move_x += 0.3;
        if(move_x > 5.0) move_x = 5.0;
    }
    else {
        move_x -= 0.3;
        if(move_x <= 0.0) move_x = 0.0;
    }


    float mx = 0.0;
    if(orientState == PLAYER_LEFT) {
        mx = move_x * -1;
    }
    else {
        mx = move_x;
    }

    asPlayer.transform->translateFor(mx,0,0);
}
