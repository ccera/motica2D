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

enum _PlayerAnimationState {
    PLAYER_RUNNING,
    PLAYER_STANDING,
    PLAYER_JUMPING,
    PLAYER_TURNING,
    PLAYER_BEGIN_RUNNING,
    PLAYER_END_BEGIN_RUNNING,
    PLAYER_END_RUNNING,
    PLAYER_TURN
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
    bool moving;
    float move_x;
};

#endif // PLAYER_H
