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

#include "sprite.h"

Sprite::Sprite() :
    QObject(), Model(NULL, texture, "")
{
}

Sprite::Sprite(Texture *texture) :
    QObject(), Model(NULL, texture, "")
{
}

void Sprite::setTexture(Texture *p_texture)
{
    this->texture = p_texture;
}

void Sprite::setName(const QString &p_name)
{
    this->name = p_name;
}

void Sprite::setPosition(const QVector3D &p)
{
    this->transform->setPosition(p.x(), p.y(), p.z());
}

QVector3D Sprite::position() const
{
    return QVector3D(this->transform->x, this->transform->y, this->transform->z);
}

void Sprite::onPicked()
{
    qDebug() << "Sprite::onPicked()" << this->model_id;
}
