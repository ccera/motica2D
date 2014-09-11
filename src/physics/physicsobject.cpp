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

#include "physicsobject.h"

PhysicsObject::PhysicsObject(PhysicsWorld *world)
{
    m_world = world;
}

void PhysicsObject::setPosition(float x, float y)
{
    body->p = cpv(x, y);
    if(this->bodyState == PHYSICSBODY_STATIC) {
        cpSpaceReindexStatic(m_world->space);
    }
}

QVector2D PhysicsObject::getPosition()
{
    return QVector2D(body->p.x, body->p.y);
}

void PhysicsObject::setRotation(float deg)
{
    cpBodySetAngle(body, degToRad(deg));
    if(this->bodyState == PHYSICSBODY_STATIC) {
        cpSpaceReindexStatic(m_world->space);
    }
}

float PhysicsObject::getRotation()
{
    return radToDeg(cpBodyGetAngle(body));
}
