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

#include "texture.h"

Texture::Texture() :
    QObject()
{
}

Texture::Texture(const QString &file) :
    QObject()
{
    this->setFile(file);
    this->textureData.load(file);
}

void Texture::setFile(const QString &file)
{
    filename = file;
    name = file;
    this->textureData.load(file);
}

QImage* Texture::GetTextureData()
{
    return &this->textureData;
}

int Texture::TextureWidth()
{
    return this->textureData.width();
}

int Texture::TextureHeight()
{
    return this->textureData.height();
}
