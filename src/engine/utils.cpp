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

#include "utils.h"

float degToRad(float deg)
{
    return deg * (0.01745329251f);   //pi 3.14159265359 / 180
}

float radToDeg(float rad)
{
    return rad * (57.2957796f);   //180.0f / 3.14159265359f
}

QString Resource(const QString &resource)
{
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    QString str = QCoreApplication::applicationDirPath() + "/Resursi/resources/" + resource;
    return str;
#elif __ANDROID_API__
    QString str = "assets:/Resursi/resources/" + resource;
    return str;
#elif _WIN64 || _WIN32
    QString str = QCoreApplication::applicationDirPath() + "/../Resursi/resources/" + resource;
    return str;
#elif TARGET_OS_MAC
    QString str = QCoreApplication::applicationDirPath() + "/../Resources/Resursi/resources/" + resource;
    return str;
#elif __linux
    QString str = QCoreApplication::applicationDirPath() + "/Resursi/resources/" + resource;
    return str;
#endif
}

QString Shader(const QString &shader)
{
#if OPENGLES_IOS
    QString str = QCoreApplication::applicationDirPath() +"/Shaders/Shaders_ES/" + shader;
    return str;
#elif OPENGLES_ANDRO
    QString str = "assets:/Shaders/Shaders_ES/" + shader;
    return str;
#elif TARGET_OS_MAC
    #if OPENGL32
        QString str = QCoreApplication::applicationDirPath() + "/../Resources/Shaders/Shaders_150/" + shader;
        return str;
    #elif OPENGL21
        QString str = QCoreApplication::applicationDirPath() + "/../Resources/Shaders/Shaders_120/" + shader;
        return str;
    #endif
#elif _WIN64 || _WIN32
    #if OPENGL21
        QString str = QCoreApplication::applicationDirPath() + "/../Shaders/Shaders_120/" + shader;
        return str;
    #endif
#elif __linux
    #if OPENGL21
        QString str = QCoreApplication::applicationDirPath() + "/Shaders/Shaders_120/" + shader;
        return str;
    #endif
#endif
}
