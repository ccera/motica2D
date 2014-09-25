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
    transformParent = 0;
    m_position = QVector3D(0,0,0);
    m_size = QVector3D(0,0,0);
    m_rotation = QVector3D(0,0,0);
}

void Transform::addChild(Transform* child)
{
    child->transformParent = this;

    child->m_position = this->transformMatrix.inverted() * child->getPosition();

    QMatrix4x4 sz;
    sz.scale(this->m_size);
    child->m_size = sz.inverted() * child->getSize();

    children.push_back(child);
    child->updateTransformMatrix();
}

void Transform::setPosition(const QVector3D &position)
{
    this->m_position = position;
    this->updateTransformMatrix();
}

void Transform::setRotation(const QVector3D &rotation)
{
    this->m_rotation = rotation;
    this->updateTransformMatrix();
}

void Transform::setSize(const QVector3D &size)
{
    this->m_size = size;
    this->updateTransformMatrix();
}

QVector3D Transform::getPosition()
{
    return m_position;
}

QVector3D Transform::getRotation()
{
    return m_rotation;
}

QVector3D Transform::getSize()
{
    return m_size;
}

void Transform::updateTransformMatrix()
{
    transformMatrix.setToIdentity();
    if(transformParent) {
        transformMatrix = transformParent->transformMatrix;
    }

    transformMatrix.translate(m_position);
    transformMatrix.rotate(m_rotation.x(), 1.0f, 0.0f, 0.0f);
    transformMatrix.rotate(m_rotation.y(), 0.0f, 1.0f, 0.0f);
    transformMatrix.rotate(m_rotation.z(), 0.0f, 0.0f, 1.0f);
    transformMatrix.scale(m_size.x(), m_size.y(), m_size.z());

    for(int n=0; n < children.size(); n++) {
        children.at(n)->updateTransformMatrix();
    }
}

Transform::~Transform()
{
    
}
