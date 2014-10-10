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

#include "physicsshape.h"

PhysicsShape::PhysicsShape(PhysicsWorld *world)
{
    m_world = world;
    parentGameObject = 0;
    width = 0;
    height = 0;
    diameter = 0;
    shape = 0;
    debug_draw_color = QColor(0,255,0);
}

void PhysicsShape::checkOverlappingList(cpShape *shape, cpContactPointSet *points, void *data)
{
    if(shape->data != NULL) {
        ((QList<PhysicsShape*>*)data)->append((PhysicsShape*)shape->data);
    }
}

void PhysicsShape::checkOverlappingBool(cpShape *shape, cpContactPointSet *points, void *data)
{
    if(shape->data != NULL) {
        (*((int*)data))++;
    }
}

QList<PhysicsShape*> PhysicsShape::listOverlappingObjects()
{
    QList<PhysicsShape*> lista;
    cpSpaceShapeQuery(m_world->space, this->shape, checkOverlappingList, (void*)&lista);
    return lista;
}

bool PhysicsShape::isOverlapping()
{
    int isOverlapping = 0;
    cpSpaceShapeQuery(m_world->space, this->shape, checkOverlappingBool, (void*)&isOverlapping);
    return (bool)isOverlapping;
}

void PhysicsShape::setFriction(float f)
{
    cpShapeSetFriction(shape, f);
}

void PhysicsShape::setSensor(bool b)
{
    shape->sensor = b;
}

bool PhysicsShape::getSensor()
{
    return shape->sensor;
}

float PhysicsShape::getWidth()
{
    return width;
}

float PhysicsShape::getHeight()
{
    return height;
}

float PhysicsShape::getDiameter()
{
    return diameter;
}

QVector3D PhysicsShape::getPosition()
{
    return QVector3D(shape->body->p.x, shape->body->p.y, 1.0f);
}

QVector3D PhysicsShape::getRotation()
{
    return QVector3D(0.0f, 0.0f, radToDeg(cpBodyGetAngle(shape->body)));
}
