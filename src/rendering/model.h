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

#ifndef __Model__
#define __Model__

#include <iostream>
#include "utils.h"
#include "texture.h"
#include "mesh.h"
#include "transform.h"

class Model
{
public:
    
    Model(Mesh *p_mesh, Texture *p_texture, const QString &p_name);
    Model();
    ~Model();
    void setTexture(Texture *p_texture);
    void setName(const QString &p_name);
    
    int     model_id;
    float   alpha;
    float   brightness;
    float   contrast;
    int     isVisible;
    bool    isSelectable;
    bool    isLabel;
    bool    isAnimated;
    int     currentFrame;
    int     numOfFrames;
    bool    horizontalMirror;
    bool    verticalMirror;
    Mesh      *mesh;
    Texture   *texture;
    Transform *transform;
    
    QString name;
};

#endif
