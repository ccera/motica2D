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
#include <QScriptEngine>
#include "engine.h"
#include "mygame.h"
#include "settings.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    Engine *engine = new Engine();
    //engine->createWindow(1024,512);
    //engine->showWindow();

    // SCRIPTING test-------
    QFile f1(Resource("scripts/drugi.js"));
    f1.open(QFile::ReadOnly | QFile::Text);
    QByteArray ba1 = f1.readAll();

    QFile f2(Resource("scripts/main.js"));
    f2.open(QFile::ReadOnly | QFile::Text);
    QByteArray ba2 = f2.readAll();

    QScriptEngine scriptEngine;

    QScriptValue objValue = scriptEngine.newQObject(engine);
    scriptEngine.globalObject().setProperty("engine", objValue);

    QScriptProgram program1(ba1.data());
    QScriptProgram program2(ba2.data());
    scriptEngine.evaluate(program1);
    scriptEngine.evaluate(program2);
    //----------------------

    //MyGame *mygame = new MyGame(engine);

    return app.exec();
}
