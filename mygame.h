#ifndef MYGAME_H
#define MYGAME_H

#include <QObject>
#include "moengine.h"
#include "texture.h"
#include "sprite.h"
#include "utils.h"
#include "gameobject.h"

class MyGame : GameObject
{
    Q_OBJECT
public:
    explicit MyGame(MoEngine *m_engine = 0);
        void update(float dt);

signals:

public slots:

private:
    MoEngine *m_engine;
    Texture planet;
    Texture bottle;
    Texture background;
    Texture fish;
    Sprite sprBackground;
    Sprite sprPlanet;
    Sprite sprBottle;
    Sprite sprFish;

    Sprite sprNiz[1000];
    float animTm;
};

#endif // MYGAME_H
