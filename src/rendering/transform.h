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

#ifndef __Transform__
#define __Transform__

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector3D>

class Transform
{
    friend class Scene;
    
public:
    Transform();
    ~Transform();
        
    void addChild(Transform* child);
    void setPosition(const QVector3D &position);
    void setWorldPosition(const QVector3D &position);
    void setSize(const QVector3D &size);
    void setWorldSize(const QVector3D &size);
    void setRotation(const QVector3D &rotation);
    void setWorldRotation(const QVector3D &rotation);
    void setRotation(const QQuaternion &rotation);
    // TODO: setWorldRotation

    QVector3D getPosition();
    // TODO: getWorldPosition
    QVector3D getSize();
    // TODO: getWorldSize
    QVector3D getRotation();
     // TODO: getWorldRotation
    QQuaternion getRotationQuaternion();

    Transform *transformParent;
    QMatrix4x4 transformMatrix;
    QVector<Transform*> children;

private:
    bool isDirty;
    void updateTransformMatrix();

    QVector3D m_size;
    QVector3D m_position;
    QVector3D m_rotation;
    QQuaternion m_quat_rotation;
};

#endif
