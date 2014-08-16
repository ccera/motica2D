#ifndef MYGAME_H
#define MYGAME_H

#include <QObject>
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "utils.h"
#include "gameevent.h"

class MyGame : public QObject, GameEvent
{
    Q_OBJECT
public:
    explicit MyGame(QObject *parent = 0);
        void update(float dt);

signals:

public slots:
        void updateSlot(float dt);

private:
    Game game;
    Texture planet;
    Texture bottle;
    Texture background;
    Texture fish;
    Sprite sprBackground;
    Sprite sprPlanet;
    Sprite sprBottle;
    Sprite sprFish;

    Sprite sprNiz[100];
    QPropertyAnimation animNiz[100];
    QPropertyAnimation animation;
    float animTm;
};

#endif // MYGAME_H
