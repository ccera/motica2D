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

#include "physicsbody.h"

PhysicsBody::PhysicsBody(PhysicsWorld *world)
{
    m_world = world;
    width = 0;
    height = 0;
    diameter = 0;
    mass = 0;
    body = 0;
}

void PhysicsBody::setPosition(const QVector3D &position)
{
    body->p = cpv(position.x(), position.y());
    if(bodyState == PHYSICS_BODY_STATIC) {
        cpSpaceReindexStatic(m_world->space);
    }
    else {
        cpSpaceReindexShapesForBody(m_world->space, this->body);
    }
}

void PhysicsBody::setRotation(const QVector3D &rotation)
{
    cpBodySetAngle(body, degToRad(rotation.z()));
    if(bodyState == PHYSICS_BODY_STATIC) {
        cpSpaceReindexStatic(m_world->space);
    }
    else {
        cpSpaceReindexShapesForBody(m_world->space, this->body);
    }
}

QVector3D PhysicsBody::getPosition()
{
    return QVector3D(body->p.x, body->p.y, 1.0f);
}

QVector3D PhysicsBody::getRotation()
{
    return QVector3D(0.0f, 0.0f, radToDeg(cpBodyGetAngle(body)));
}

void PhysicsBody::applyForce(float x, float y)
{
    cpBodyApplyForce(body, cpv(x,y), cpvzero);
}

void PhysicsBody::applyForceWithOffset(float x, float y, float offset_x, float offset_y)
{
    cpBodyApplyForce(body, cpv(x,y), cpv(offset_x, offset_y));
}

void PhysicsBody::applyImpulse(float x, float y)
{
    cpBodyApplyImpulse(body, cpv(x,y), cpvzero);
}

void PhysicsBody::applyImpulseWithOffset(float x, float y, float offset_x, float offset_y)
{
    cpBodyApplyImpulse(body, cpv(x,y), cpv(offset_x, offset_y));
}


QVector2D PhysicsBody::getVelocity()
{
    cpVect v = cpBodyGetVel(body);
    return QVector2D(v.x, v.y);
}

void PhysicsBody::setVelocity(float x, float y)
{
    cpBodySetVel(body, cpv(x,y));
}

void PhysicsBody::setMaxVelocity(float max)
{
    cpBodySetVelLimit(body, max);
}

float PhysicsBody::getWidth()
{
    return width;
}

float PhysicsBody::getHeight()
{
    return height;
}

float PhysicsBody::getDiameter()
{
    return diameter;
}

void PhysicsBody::disableRotation()
{
    cpBodySetMoment(body, INFINITY);
}

void PhysicsBody::enableRotation()
{
    if(bodyShape == PHYSICS_BODYSHAPE_BOX) {
        float i = cpMomentForBox(mass, width, height);
        cpBodySetMoment(body, i);
    }

    if(bodyShape == PHYSICS_BODYSHAPE_CIRCLE) {
        float i = cpMomentForCircle(mass, 0.0f, diameter, cpvzero);
        cpBodySetMoment(body, i);
    }
}

void PhysicsBody::setMomentOfInertia(float i)
{
    cpBodySetMoment(body, i);
}
