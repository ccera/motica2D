//
//  Copyright (c) 2014 Emir Cerić. All rights reserved.
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

#include <QtGui/QGuiApplication>
#include <QDebug>
#include "game.h"
#include "texture.h"
#include "sprite.h"
#include "label.h"
#include "utils.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    Game game;
    game.setWindowSize(1024, 500);
    game.setAnimating(true);
    game.setBackgroundColor(0.5f, 0.0f, 0.3f, 1.0f);
    game.showWindow();

    Texture tex(Resource("textures/Crate.png"));
    game.addTexture(&tex);

    Texture background(Resource("textures/Bananaz.jpg"));
    game.addTexture(&background);

    Sprite sprBack(&background);
    sprBack.transform->setPosition(512,250,-10.0f);
    sprBack.transform->setSize(1024,500,0);
    game.addSprite(&sprBack);

    Sprite sprite(&tex);
    sprite.transform->setPosition(300,300,10.0f);
    sprite.transform->setSize(100,100,0);
    game.addSprite(&sprite);

    Label labela;
    labela.text = "Motica2D";
    labela.fontSize = 30;
    labela.transform->setSize(200,50,0);
    labela.transform->setPosition(512,450,1);
    game.addLabel(&labela);
    labela.color.setRgb(255,255,255);

    return app.exec();
}
