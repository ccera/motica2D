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

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <math.h>
#include <string.h>
#include <QtOpenGL>
#include <QString>

#if _WIN64 || _WIN32 || (__linux && !__ANDROID_API__)
    #define OPENGL21 1
    #include <qopengl.h>
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #define OPENGLES_IOS 1
    #elif TARGET_OS_MAC
         #define OPENGL21 1
         //#define OPENGL32 1
    #endif
#elif __ANDROID_API__
    #define OPENGLES_ANDRO 1
#elif __linux
    #include <qopengl.h>
#endif

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define PI 3.14159265359

enum _ViewportType {
    VIEWPORT_PIXEL,
    VIEWPORT_STRECH_X,
    VIEWPORT_STRECH_Y,
    VIEWPORT_STRECH_XY,
};
typedef enum _ViewportType ViewportType;

float degToRad(float deg);
float radToDeg(float rad);
QString Resource(const QString &resource);
QString Shader(const QString &shader);

#endif // UTILS_H
