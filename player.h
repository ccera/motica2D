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

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QStateMachine>
#include "window.h"
#include "texture.h"
#include "sprite.h"
#include "utils.h"
#include "gameobject.h"
#include "animatedsprite.h"
#include "statequeueinterface.h"
#include "statequeue.h"
#include "physicsbody.h"
#include "chipmunk.h"
#include "engine.h"

enum _GameObjects {
    GAME_FLOOR,
    GAME_PLAYER,
    GAME_POWERUP
};
typedef enum _GameObjects GameObjects;

enum _PlayerAnimationState {
    PLAYER_RUNNING,
    PLAYER_STANDING,
    PLAYER_FLYING,
    PLAYER_TURNING,
    PLAYER_BEGIN_RUNNING,
    PLAYER_END_BEGIN_RUNNING,
    PLAYER_END_RUNNING,
    PLAYER_TURN,
    PLAYER_START_JUMP_FROM_RUN,
    PLAYER_END_JUMP_FROM_RUN,
    PLAYER_TURN_FROM_RUN
};
typedef enum _PlayerAnimationState PlayerAnimationState;


enum _PlayerOrientationState {
    PLAYER_LEFT,
    PLAYER_RIGHT
};
typedef enum _PlayerOrientationState PlayerOrientationState;


enum _ControlsState {
    CONTROLS_LEFT,
    CONTROLS_RIGHT,
    CONTROLS_NOTHING,
    CONTROLS_UP,
    CONTROLS_DOWN,
    CONTROLS_FEET_TOUCHING,
    CONTROLS_LEFT_UP,
    CONTROLS_RIGHT_UP
};
typedef enum _ControlsState ControlsState;

enum _PlayerState {
    ON_FEET,
    RUNNING,
    TURNING,
    FALLING,
    FELL_DOWN,
    FLYING
};
typedef enum _PlayerState PlayerState;


class Player : public GameObject, StateQueueInterface
{
    Q_OBJECT
public:
    explicit Player(Engine *engine = 0);
    virtual void update(float dt);
    QVector2D rotateAround(const QVector2D &pos, const QVector2D &around, float angle);

    void checkKey();
    void checkState();
    void debugPrintState();

    PlayerAnimationState animState;
    PlayerOrientationState orientState;
    ControlsState controlsState;
    PlayerState playerState;

    virtual void onStateEntered(int animState);
    virtual void onStateExited(int animState);
    virtual void collide(PhysicsBody *with);

signals:

public slots:

private:
    Engine *m_engine;
    Texture *txPlayer;
    AnimatedSprite *asPlayer;
    Texture *planet;
    Sprite *sprPlanet;
    StateQueue stateQueue;
    PhysicsBody *playerBody;
    PhysicsBody *feetSensor;
    PhysicsBody *headSensorL;
    PhysicsBody *headSensorLU;
    PhysicsBody *headSensorR;
    PhysicsBody *headSensorRU;
    PhysicsBody *headSensorT;

    PhysicsShape *playerShape;


    bool feetTouching;
    bool headTouchingL;
    bool headTouchingLU;
    bool headTouchingR;
    bool headTouchingRU;
    bool headTouchingT;
    bool bodyTouching;
    int turnTimer;
    int flyTimer;
    int fellDownTimer;
    int onFeetTimer;
    bool jumpAllowed;
};

#endif // PLAYER_H
