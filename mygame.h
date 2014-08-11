#ifndef MYGAME_H
#define MYGAME_H

#include <QObject>
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "label.h"
#include "utils.h"

class MyGame : public QObject
{
    Q_OBJECT
public:
    explicit MyGame(QObject *parent = 0);

signals:

public slots:
    void update(float dt);

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
    Label labela;

    QPropertyAnimation animation;
    float animTm;
};

#endif // MYGAME_H
