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
#include "moengine.h"
#include "texture.h"
#include "sprite.h"
#include "utils.h"
#include "gameobject.h"
#include "animatedsprite.h"
#include "statequeueinterface.h"
#include "statequeue.h"
#include "physicsobject.h"
#include "chipmunk.h"

enum _PlayerAnimationState {
    PLAYER_RUNNING,
    PLAYER_STANDING,
    PLAYER_JUMPING,
    PLAYER_TURNING,
    PLAYER_BEGIN_RUNNING,
    PLAYER_END_BEGIN_RUNNING,
    PLAYER_END_RUNNING,
    PLAYER_TURN,
    PLAYER_JUMP_FROM_RUN,
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
    CONTROLS_DOWN
};
typedef enum _ControlsState ControlsState;


class Player : public GameObject, StateQueueInterface
{
    Q_OBJECT
public:
    explicit Player(MoEngine *engine = 0);
    virtual void update(float dt);

    void checkKey();
    void checkState();

    PlayerAnimationState animState;
    PlayerOrientationState orientState;
    ControlsState controlsState;

    virtual void onStateEntered(int animState);
    virtual void onStateExited(int animState);

signals:

public slots:

private:
    MoEngine *m_engine;
    Texture txPlayer;
    AnimatedSprite asPlayer;
    StateQueue stateQueue;
    PhysicsObject *playerBody;
    bool moving;
    bool jumping;
    float move_x;
    float move_y;
};

#endif // PLAYER_H
