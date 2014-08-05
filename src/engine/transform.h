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
    void updateTransformMatrix();
    void setPosition(float x, float y, float z);
    void translateFor(float _x, float _y, float _z);
    void setRotation(float x_angle, float y_angle, float z_angle);
    void rotateFor(float x_angle, float y_angle, float z_angle);
    void rotateAroundTo(float _x, float _y, float _z, float x_angle, float y_angle, float z_angle);
    void setSize(float x_size, float y_size, float z_size);
    void resizeFor(float x_size, float y_size, float z_size);

    Transform *parent;
    QMatrix4x4 transformMatrix;
    float x;
    float y;
    float z;
    float size_x;
    float size_y;
    float size_z;
    float rot_x;
    float rot_y;
    float rot_z;
    
private:
    bool isDirty;
};

#endif
