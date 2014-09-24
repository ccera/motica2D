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

#include "transform.h"

Transform::Transform()
{
    m_size_x = 1.0f;
    m_size_y = 1.0f;
    m_size_z = 1.0f;
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = 0.0f;
    m_rot_x = 0.0f;
    m_rot_y = 0.0f;
    m_rot_z = 0.0f;
    parent = 0;
    transformMatrix.setToIdentity();
}

void Transform::addChild(Transform* child)
{
    child->parent = this;
/*
    float szx;
    float szy;
    float szz;

    if(this->m_size_x == 0.0f) szx = 1.0f;
    else szx = this->m_size_x;

    if(this->m_size_y == 0.0f) szy = 1.0f;
    else szy = this->m_size_y;

    if(this->m_size_z == 0.0f) szz = 1.0f;
    else szz = this->m_size_z;

    child->m_size_x = child->m_size_x / szx;
    child->m_size_y = child->m_size_y / szy;
    child->m_size_z = child->m_size_z / szz;

    child->m_x = child->m_x / szx;
    child->m_y = child->m_y / szy;
    child->m_z = child->m_z / szz;
*/
//TODO Rotacije ne treba ??
//    child->rot_x = child->rot_x / this->rot_x;
//    child->rot_y = child->rot_y / this->rot_z;
//    child->rot_z = child->rot_z / this->rot_z;
}

void Transform::updateTransformMatrix()
{
    if(isDirty) {
        //transformMatrix.setToIdentity();

        float szx = 1.0f;
        float szy = 1.0f;
        float szz = 1.0f;

        if(parent) {
            if(parent->m_size_x == 0.0f) szx = 1.0f;
            else szx = parent->m_size_x;

            if(parent->m_size_y == 0.0f) szy = 1.0f;
            else szy = parent->m_size_y;

            if(parent->m_size_z == 0.0f) szz = 1.0f;
            else szz = parent->m_size_z;

            parent->updateTransformMatrix();
            transformMatrix =  transformMatrix * parent->transformMatrix;
        }

        //transformMatrix.translate(m_x/abs(szx), m_y/abs(szy), m_z/abs(szz));
        //transformMatrix.rotate(m_rot_x, 1.0f, 0.0f, 0.0f);
        //transformMatrix.rotate(m_rot_y, 0.0f, 1.0f, 0.0f);
        //transformMatrix.rotate(m_rot_z, 0.0f, 0.0f, 1.0f);
        //transformMatrix.scale(m_size_x/abs(szx), m_size_y/abs(szy), m_size_z/abs(szz));
        isDirty = false;
        //qDebug() << m_x << m_y << "  -  " << transformMatrix.map(QPoint(0,0));
   }
}

void Transform::setPosition(float _x, float _y, float _z)
{
    isDirty = true;
    transformMatrix.translate(_x, _y, _z);
    //m_x = _x;
    //m_y = _y;
    //_z = _z;

}

void Transform::translateFor(float _x, float _y, float _z)
{
    isDirty = true;
    m_x = m_x + _x;
    m_y = m_y + _y;
    m_z = m_z + _z;
}

void Transform::setRotation(float x_angle, float y_angle, float z_angle)
{
    isDirty = true;
    m_rot_x = x_angle;
    m_rot_y = y_angle;
    m_rot_z = z_angle;
}

void Transform::rotateFor(float x_angle, float y_angle, float z_angle)
{
    isDirty = true;
    m_rot_x += x_angle;
    m_rot_y += y_angle;
    m_rot_z += z_angle;
}

void Transform::rotateAroundTo(float _x, float _y, float _z, float x_angle, float y_angle, float z_angle)
{
    isDirty = true;
    QVector3D p(m_x, m_y, m_z);
    QVector3D a(_x, _y, _z);
    QMatrix4x4 m;

    p-=a;
    m.rotate(-x_angle, 1.0f, 0.0f, 0.0f);
    m.rotate(-y_angle, 0.0f, 1.0f, 0.0f);
    m.rotate(-z_angle, 0.0f, 0.0f, 1.0f);
    p = p * m;
    p+=a;

    m_rot_x += x_angle;
    m_rot_y += y_angle;
    m_rot_z += z_angle;

    m_x = p.x();
    m_y = p.y();
    m_z = p.z();
}

void Transform::setSize(float x_size, float y_size, float z_size)
{
    isDirty = true;
    transformMatrix.scale(x_size, y_size, z_size);
    //m_size_x = x_size;
    //m_size_y = y_size;
    //m_size_z = z_size;
}

void Transform::resizeFor(float x_size, float y_size, float z_size)
{
    isDirty = true;
    m_size_x = m_size_x + x_size;
    m_size_y = m_size_y + y_size;
    m_size_z = m_size_z + z_size;
}

float Transform::x() const
{
    return m_x;
}

float Transform::y() const
{
    return m_y;
}

float Transform::z() const
{
    return m_z;
}

float Transform::size_x() const
{
    return m_size_x;
}

float Transform::size_y() const
{
    return m_size_y;
}

float Transform::size_z() const
{
    return m_size_z;
}

float Transform::rot_x() const
{
    return m_rot_x;
}

float Transform::rot_y() const
{
    return m_rot_y;
}

float Transform::rot_z() const
{
    return m_rot_z;
}

Transform::~Transform()
{
    
}
