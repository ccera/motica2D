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

#ifndef PHYSICSSHAPE_H
#define PHYSICSSHAPE_H

#include "physicsworld.h"
#include <QVector2D>
#include "utils.h"
#include "chipmunk.h"
#include "gameobject.h"
#include "types.h"
#include "transform.h"
#include "physicsbody.h"

class PhysicsWorld;
class GameObject;

class PhysicsShape
{
    friend class PhysicsWorld;

public:
    PhysicsShape(PhysicsWorld *world);

    QList<PhysicsShape*> listOverlappingObjects();
    bool        isOverlapping();
    void        setFriction(float f);
    void        setSensor(bool b);
    bool        getSensor();
    float       getWidth();
    float       getHeight();
    float       getDiameter();
    QVector3D   getPosition();
    QVector3D   getRotation();

    cpShape *shape;
    PhysicsObjectShapeType shapeType;
    GameObject *parentGameObject;
    int userType;
    QString name;
    QVector2D offset;

    QColor debug_draw_color;

private:
    static void checkOverlappingList(cpShape *shape, cpContactPointSet *points, void *data);
    static void checkOverlappingBool(cpShape *shape, cpContactPointSet *points, void *data);
    PhysicsWorld *m_world;
    float width;
    float height;
    float diameter;
};

#endif // PHYSICSSHAPE_H
