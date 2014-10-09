//
//  Copyright (c) 2014 Emir Cerić, Dejan Omasta. All rights reserved.
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
#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <QObject>
#include <QMatrix3x3>
#include "scene.h"
#include "utils.h"
#include "scene.h"
#include "transform.h"
#include "model.h"
#include "texture.h"

class AnimatedSprite : public QObject, public Model
{
    Q_OBJECT

public:
    AnimatedSprite();
    AnimatedSprite(int m_rows, int m_columns, Texture *texture);

    void setTexture(Texture *p_texture);
    void setName(const QString &p_name);
    void setRows(int r);
    void setColumns(int c);
    void setFrameLength(float msec);
    void onPicked();
    void update(float dt);
    void setLoop(int from, int to);


signals:

public slots:

private:
    void setCurrentFrame(int n);
    int  getCurrentFrame();

    int counter;
    int m_frame_length;
    int m_loop_from;
    int m_loop_to;
    int direction;
    int m_rows;
    int m_columns;
    int m_currentFrame;
    int m_numOfFrames;

};

#endif // ANIMATEDSPRITE_H
