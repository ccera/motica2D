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
#include "engine.h"
#include "mygame.h"
#include "settings.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);



    Engine *engine = new Engine();
    engine->createWindow(1024,512);
    engine->showWindow();

    MyGame *mygame = new MyGame(engine);

    return app.exec();
}
