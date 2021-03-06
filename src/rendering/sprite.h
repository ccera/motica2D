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

#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include "scene.h"
#include "utils.h"
#include "scene.h"
#include "transform.h"
#include "model.h"
#include "texture.h"

class Sprite : public QObject, public Model
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition)

public:
    Sprite();
    Sprite(Texture *texture);

    void setTexture(Texture *p_texture);
    void setName(const QString &p_name);
    void onPicked();

    void setPosition(const QVector3D &p);
    QVector3D position() const;

signals:

public slots:

};

#endif // SPRITE_H
