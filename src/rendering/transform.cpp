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
}

void Transform::addChild(Transform* child)
{
    float szx = 1.0;
    float szy = 1.0;
    float szz = 1.0;

    if(this->m_size.x() == 0.0f) szx = 1.0f;
    else szx = this->m_size.x();

    if(this->m_size.y() == 0.0f) szy = 1.0f;
    else szy = this->m_size.y();

    if(this->m_size.z() == 0.0f) szz = 1.0f;
    else szz = this->m_size.z();

    child->transformParent = this;

    child->setPosition(QVector3D(child->m_position.x()/szx, child->m_position.y()/szy, child->m_position.z()/szz));
    child->setSize(QVector3D(child->m_size.x()/szx, child->m_size.y()/szy, child->m_size.z()/szz));

    children.push_back(child);
    child->updateTransformMatrix();
}

void Transform::setPosition(const QVector3D &position)
{
    this->m_position = position;
    this->updateTransformMatrix();
}

void Transform::setWorldPosition(const QVector3D &position)
{
    float szx = 1.0f;
    float szy = 1.0f;
    float szz = 1.0f;

    if(transformParent) {
        if(transformParent->getSize().x() == 0.0f) szx = 1.0f;
        else szx = transformParent->getSize().x();

        if(transformParent->getSize().y() == 0.0f) szy = 1.0f;
        else szy = transformParent->getSize().y();

        if(transformParent->getSize().z() == 0.0f) szz = 1.0f;
        else szz = transformParent->getSize().z();
    }

    this->m_position = QVector3D(position.x()/szx, position.y()/szy, position.z()/szz);
    this->updateTransformMatrix();
}

void Transform::setRotation(const QVector3D &rotation)
{
    m_quat_rotation = m_quat_rotation.fromAxisAndAngle(1.0f, 0.0f, 0.0f, rotation.x());
    m_quat_rotation = m_quat_rotation * m_quat_rotation.fromAxisAndAngle(0.0f, 1.0f, 0.0f, rotation.y());
    m_quat_rotation = m_quat_rotation * m_quat_rotation.fromAxisAndAngle(0.0f, 0.0f, 1.0f, rotation.z());
    this->m_rotation = rotation;
    this->updateTransformMatrix();
}

void Transform::setRotation(const QQuaternion &rotation)
{
    this->m_quat_rotation = rotation;
    this->updateTransformMatrix();
}

void Transform::setSize(const QVector3D &size)
{
    this->m_size = size;
    this->updateTransformMatrix();
}

void Transform::setWorldSize(const QVector3D &size)
{
    float szx = 1.0f;
    float szy = 1.0f;
    float szz = 1.0f;

    if(transformParent) {
        if(transformParent->getSize().x() == 0.0f) szx = 1.0f;
        else szx = transformParent->getSize().x();

        if(transformParent->getSize().y() == 0.0f) szy = 1.0f;
        else szy = transformParent->getSize().y();

        if(transformParent->getSize().z() == 0.0f) szz = 1.0f;
        else szz = transformParent->getSize().z();
    }

    this->m_size = QVector3D(size.x()/szx, size.y()/szy, size.z()/szz);
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

QQuaternion Transform::getRotationQuaternion()
{
    return m_quat_rotation;
}

QVector3D Transform::getSize()
{
    return m_size;
}

void Transform::updateTransformMatrix()
{
    transformMatrix.setToIdentity();

    transformMatrix.translate(m_position);
    transformMatrix.rotate(m_quat_rotation);
    transformMatrix.scale(m_size);

    if(transformParent) {
        transformMatrix = transformParent->transformMatrix * transformMatrix;
    }

    for(int n=0; n < children.size(); n++) {
        children.at(n)->updateTransformMatrix();
    }
}

Transform::~Transform()
{
    
}
