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

#include "model.h"

Model::Model(Mesh *p_mesh, Texture *p_texture, const QString &p_name)
{
    name = p_name;
    mesh = p_mesh;
    alpha = 1.0f;
    brightness = 0.0f;
    contrast = 1.0f;
    isSelectable = true;
    model_id = 0;
    isVisible = true;
    this->setTexture(p_texture);
    transform = new Transform();
    isLabel = false;
    isAnimated = false;
    currentFrame = 0;
    numOfFrames = 0;
    horizontalMirror = false;
    verticalMirror = false;
}

Model::Model()
{
    alpha = 1.0f;
    brightness = 0.0f;
    contrast = 1.0f;
    isSelectable = true;
    model_id = 0;
    isVisible = true;
    texture = NULL;
    this->setName("");
    transform = new Transform();
    isLabel = false;
    isAnimated = false;
    currentFrame = 0;
    numOfFrames = 0;
    horizontalMirror = false;
    verticalMirror = false;
}

void Model::setTexture(Texture *p_texture)
{
    texture = p_texture;
}

void Model::setName(const QString &p_name)
{
    name = p_name;
}

Model::~Model()
{
    delete transform;
}
