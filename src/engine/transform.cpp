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
    size_x = 1.0;
    size_y = 1.0;
    size_z = 1.0;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    rot_x = 0.0f;
    rot_y = 0.0f;
    rot_z = 0.0f;
    parent = 0;
}

void Transform::addChild(Transform* child)
{
    child->parent = this;
/*
    child->size_x = child->size_x / this->size_x;
    child->size_y = child->size_y / this->size_y;
    child->size_z = child->size_z / this->size_z;

    child->x = child->x / this->x;
    child->y = child->y / this->y;
    child->z = child->z / this->z;

    child->rot_x = child->rot_x / this->rot_x;
    child->rot_y = child->rot_y / this->rot_z;
    child->rot_z = child->rot_z / this->rot_z;
*/
}

void Transform::updateTransformMatrix()
{
    if(isDirty) {
        transformMatrix.setToIdentity();
        if(parent) {
            parent->updateTransformMatrix();
            transformMatrix =  transformMatrix * parent->transformMatrix;
        }

        transformMatrix.translate(x, y, z);
        transformMatrix.rotate(rot_x, 1.0f, 0.0f, 0.0f);
        transformMatrix.rotate(rot_x, 0.0f, 1.0f, 0.0f);
        transformMatrix.rotate(rot_z, 0.0f, 0.0f, 1.0f);
        transformMatrix.scale(size_x, size_y, size_z);
        isDirty = false;
   }
}

void Transform::setPosition(float _x, float _y, float _z)
{
    isDirty = true;
    x = _x;
    y = _y;
    z = _z;
}

void Transform::translateFor(float _x, float _y, float _z)
{
    isDirty = true;
    x = x + _x;
    y = y + _y;
    z = z + _z;
}

void Transform::setRotation(float x_angle, float y_angle, float z_angle)
{
    isDirty = true;
    rot_x = x_angle;
    rot_y = y_angle;
    rot_z = z_angle;
}

void Transform::rotateFor(float x_angle, float y_angle, float z_angle)
{
    isDirty = true;
    rot_x += x_angle;
    rot_y += y_angle;
    rot_z += z_angle;
}

void Transform::rotateAroundTo(float _x, float _y, float _z, float x_angle, float y_angle, float z_angle)
{
    isDirty = true;
    QVector3D p(x, y, z);
    QVector3D a(_x, _y, _z);
    QMatrix4x4 m;

    p-=a;
    m.rotate(-x_angle, 1.0f, 0.0f, 0.0f);
    m.rotate(-y_angle, 0.0f, 1.0f, 0.0f);
    m.rotate(-z_angle, 0.0f, 0.0f, 1.0f);
    p = p * m;
    p+=a;

    rot_x += x_angle;
    rot_y += y_angle;
    rot_z += z_angle;

    x = p.x();
    y = p.y();
    z = p.z();
}

void Transform::setSize(float x_size, float y_size, float z_size)
{
    isDirty = true;
    size_x = x_size;
    size_y = y_size;
    size_z = z_size;
}

void Transform::resizeFor(float x_size, float y_size, float z_size)
{
    isDirty = true;
    size_x = size_x + x_size;
    size_y = size_y + y_size;
    size_z = size_z + z_size;
}

Transform::~Transform()
{
    
}
