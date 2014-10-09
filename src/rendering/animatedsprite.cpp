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

#include "animatedsprite.h"

AnimatedSprite::AnimatedSprite() :
    QObject(), Model(NULL, texture, "")
{
    m_currentFrame = 0;
    m_numOfFrames = 1;
    m_frame_length = 3;
    m_loop_from = 0;
    m_loop_to = 0;
    counter = 0;
    direction = 1;
    m_rows = 1;
    m_columns = 1;
}

AnimatedSprite::AnimatedSprite(int rows, int columns, Texture *texture) :
    QObject(), Model(NULL, texture, "")
{
    m_currentFrame = 0;
    m_numOfFrames = rows * columns;
    m_frame_length = 3;
    m_loop_from = 0;
    m_loop_to = 0;
    counter = 0;
    direction = 1;
    m_rows = rows;
    m_columns = columns;
}

void AnimatedSprite::setFrameLength(float msec)
{
    if(msec < 0) {
        m_frame_length = 0;
    }
    else {
        m_frame_length = msec;
    }
}

void AnimatedSprite::setRows(int r)
{
    m_rows = r;
    m_numOfFrames = m_rows * m_columns;
}

void AnimatedSprite::setColumns(int c)
{
    m_columns = c;
    m_numOfFrames = m_rows * m_columns;
}

void AnimatedSprite::setTexture(Texture *p_texture)
{
    this->texture = p_texture;
}

void AnimatedSprite::setName(const QString &p_name)
{
    this->name = p_name;
}

void AnimatedSprite::setCurrentFrame(int n)
{
    if(n > (m_numOfFrames-1)) {
        m_currentFrame = (m_numOfFrames-1);
    }
    else if(n < 0) {
        m_currentFrame = 0;
    }
    else {
        m_currentFrame = n;
    }


    // Transform trenutni UV da bi se dobio trazeni frame
    UVTransform.setToIdentity();

    float csz = (1.0 / m_columns);
    float rsz = (1.0 / m_rows);
    QMatrix3x3 size;
    size.setToIdentity();
    size(0,0) = csz; //x-size
    size(1,1) = rsz; //y-size

    int b = m_currentFrame / m_columns;
    int c = m_currentFrame - (b*m_columns);
    QMatrix3x3 pos;
    pos.setToIdentity();
    pos(0,2) = (csz * c); //x-pos
    pos(1,2) = (rsz * ((m_rows-1)-b)); //y-pos
    pos = pos.transposed();

    UVTransform = size * pos ;
}

int AnimatedSprite::getCurrentFrame()
{
    return m_currentFrame;
}

void AnimatedSprite::onPicked()
{
    qDebug() << "AnimatedSprite::onPicked()" << this->model_id;
}

void AnimatedSprite::setLoop(int from, int to)
{
    if((to - from) > 0) {
        direction = 1;
    }
    else if ((to - from) < 0) {
        direction = -1;
    }
    else {
        direction = 0;
    }
    setCurrentFrame(from);
    m_loop_from = from;
    m_loop_to = to;
    counter = 0;
}

void AnimatedSprite::update(float dt)
{
    if(direction == 0) {
        return;
    }

    if(counter  == (m_frame_length-1)) {
        setCurrentFrame(getCurrentFrame() + direction);

        if( (direction == 1 && getCurrentFrame() > m_loop_to) || (direction == -1 && getCurrentFrame() < m_loop_to) ) {
                setCurrentFrame(m_loop_from);
        }
    }

    counter = (counter + 1) % m_frame_length;
}
