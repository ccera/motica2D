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

#ifndef LABEL_H
#define LABEL_H

#include <QObject>
#include "scene.h"
#include "utils.h"
#include "scene.h"
#include "transform.h"
#include "model.h"
#include "texture.h"

class Label : public QObject, public Model
{
    Q_OBJECT
public:
    explicit Label(QObject *parent = 0);

    void onPicked();

    QString fontName;
    float fontSize;
    QString text;
    QColor color;
    QColor backgroundColor;

signals:

public slots:

};

#endif // LABEL_H
