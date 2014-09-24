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

#ifndef __Mesh__
#define __Mesh__

#include "utils.h"

using namespace std;

class Mesh
{
public:
    Mesh(float *p_vertexData, int *p_indexData,  int p_vertexCount, int p_triangleCount, const QString &p_name);
    ~Mesh();
    
    QString name;
    float  *vertexData;
    int    *indexData;
    size_t indexDataSize;
    size_t vertexDataSize;
    int    triangleCount;
    int    vertexCount;
    GLuint IBO;
    GLuint VBO;
    GLuint VAOStd;
    GLuint VAOShd;
    GLuint VAOShdMap;
    GLuint VAOSpr;
    GLuint VAOPick;
    bool isGLBound;
};

#endif
