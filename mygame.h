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

#ifndef MYGAME_H
#define MYGAME_H

#include <QObject>
#include "moengine.h"
#include "texture.h"
#include "sprite.h"
#include "utils.h"
#include "player.h"
#include "physicsobject.h"

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

    Player *prince;
    PhysicsObject *pod;

    Sprite sprNiz[1000];
    float animTm;
};

#endif // MYGAME_H
