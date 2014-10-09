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
#include "physicsbody.h"
#include "chipmunk.h"
#include "engine.h"
#include "settings.h"

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
    PLAYER_FALLING
};
typedef enum _PlayerAnimationState PlayerAnimationState;


enum _PlayerOrientationState {
    ORIENT_LEFT,
    ORIENT_RIGHT
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

enum _PlayerMovementState {
    STANDING,
    RUNNING,
    FALLING,
    FLYING
};
typedef enum _PlayerMovementState PlayerMovementState;


class Player : public GameObject
{
    Q_OBJECT
public:
    explicit Player(Engine *engine = 0);
    virtual void update(float dt);

    void checkKey();
    void checkState();
    void debugPrintState();

    PlayerAnimationState animState;
    PlayerOrientationState orientState;
    ControlsState controlsState;
    PlayerMovementState playerState;

    virtual void collide(PhysicsBody *with);

signals:

public slots:

private:
    Engine          *m_engine;
    Texture         *txPlayer;
    AnimatedSprite  *asprPlayer;

    PhysicsBody  *playerBody;
    PhysicsShape *playerShape;
    PhysicsShape *feetSensorL;
    PhysicsShape *feetSensorR;
    PhysicsShape *bodySensorL;
    PhysicsShape *bodySensorR;
    PhysicsShape *headSensorL;
    PhysicsShape *headSensorR;

    bool feetTouchingL;
    bool feetTouchingR;
    bool headTouchingL;
    bool headTouchingR;
    bool bodyTouchingL;
    bool bodyTouchingR;
    int  onFeetTimer;
    bool jumpAllowed;

};

#endif // PLAYER_H
