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
    Texture tex;
    Texture background;
    Sprite sprBackground;
    Sprite sprCrate;
    Label labela;
};

#endif // MYGAME_H
