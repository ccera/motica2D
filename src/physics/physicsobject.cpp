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
    parentGameObject = 0;
    width = 0;
    height = 0;
    diameter = 0;
    mass = 0;
}

void PhysicsObject::update(float dt)
{
    Transform::setWorldPosition(QVector3D(body->p.x, body->p.y, 1.0f));
    //Transform::setWorldRotation(QVector3D(0.0f, 0.0f, radToDeg(cpBodyGetAngle(body))));
}

void PhysicsObject::setPosition(const QVector3D &position)
{
    Transform::setPosition(position);
    body->p = cpv(position.x(), position.y());
    if(bodyState == PHYSICSBODY_STATIC) {
        cpSpaceReindexStatic(m_world->space);
    }
    else {
        cpSpaceReindexShapesForBody(m_world->space, this->body);
    }
}

void PhysicsObject::setRotation(const QVector3D &rotation)
{
    Transform::setRotation(rotation);
    //cpBodySetAngle(body, degToRad(rotation.z()));
    if(bodyState == PHYSICSBODY_STATIC) {
        cpSpaceReindexStatic(m_world->space);
    }
    else {
        cpSpaceReindexShapesForBody(m_world->space, this->body);
    }
}

//void PhysicsObject::setSize(const QVector3D &size)
//{
//    // Nista se ne desava jer velicina se postavlja kod kreianja bodyja
//}


//void PhysicsObject::setPosition(float x, float y)
//{
//    body->p = cpv(x, y);
//    if(this->bodyState == PHYSICSBODY_STATIC) {
//        cpSpaceReindexStatic(m_world->space);
//    }
//    else {
//        cpSpaceReindexShapesForBody(m_world->space, this->body);
//    }
//}

//QVector2D PhysicsObject::getPosition()
//{
//    return QVector2D(body->p.x, body->p.y);
//}

//void PhysicsObject::setRotation(float deg)
//{
//    cpBodySetAngle(body, degToRad(deg));
//    if(this->bodyState == PHYSICSBODY_STATIC) {
//        cpSpaceReindexStatic(m_world->space);
//    }
//    else {
//        cpSpaceReindexShapesForBody(m_world->space, this->body);
//    }
//}

//float PhysicsObject::getRotation()
//{
//    return radToDeg(cpBodyGetAngle(body));
//}

void PhysicsObject::applyForce(float x, float y)
{
    cpBodyApplyForce(body, cpv(x,y), cpvzero);
}

void PhysicsObject::applyForceWithOffset(float x, float y, float offset_x, float offset_y)
{
    cpBodyApplyForce(body, cpv(x,y), cpv(offset_x, offset_y));
}

void PhysicsObject::applyImpulse(float x, float y)
{
    cpBodyApplyImpulse(body, cpv(x,y), cpvzero);
}

void PhysicsObject::applyImpulseWithOffset(float x, float y, float offset_x, float offset_y)
{
    cpBodyApplyImpulse(body, cpv(x,y), cpv(offset_x, offset_y));
}

void PhysicsObject::setFriction(float f)
{
    cpShapeSetFriction(shape, f);
}

QVector2D PhysicsObject::getVelocity()
{
    cpVect v = cpBodyGetVel(body);
    return QVector2D(v.x, v.y);
}

void PhysicsObject::setVelocity(float x, float y)
{
    cpBodySetVel(body, cpv(x,y));
}

void PhysicsObject::setMaxVelocity(float max)
{
    cpBodySetVelLimit(body, max);
}

float PhysicsObject::getWidth()
{
    return width;
}

float PhysicsObject::getHeight()
{
    return height;
}

float PhysicsObject::getDiameter()
{
    return diameter;
}

void PhysicsObject::disableRotation()
{
    cpBodySetMoment(body, INFINITY);
}

void PhysicsObject::enableRotation()
{
    if(shapeType == PHYSICSOBJECT_BOX) {
        float i = cpMomentForBox(mass, width, height);
        cpBodySetMoment(body, i);
    }

    if(shapeType == PHYSICSOBJECT_CIRCLE) {
        float i = cpMomentForCircle(mass, 0.0f, diameter, cpvzero);
        cpBodySetMoment(body, i);
    }
}

void PhysicsObject::setMomentOfInertia(float i)
{
    cpBodySetMoment(body, i);
}

void PhysicsObject::checkOverlappingList(cpShape *shape, cpContactPointSet *points, void *data)
{
    if(shape->data != NULL) {
        ((QList<PhysicsObject*>*)data)->append((PhysicsObject*)shape->data);
    }
}

void PhysicsObject::checkOverlappingBool(cpShape *shape, cpContactPointSet *points, void *data)
{
    if(shape->data != NULL) {
        (*((int*)data))++;
    }
}

QList<PhysicsObject*> PhysicsObject::listOverlappingObjects()
{
    QList<PhysicsObject*> lista;
    cpSpaceShapeQuery(m_world->space, this->shape, checkOverlappingList, (void*)&lista);
    return lista;
}

bool PhysicsObject::isOverlapping()
{
    int isOverlapping = 0;
    cpSpaceShapeQuery(m_world->space, this->shape, checkOverlappingBool, (void*)&isOverlapping);
    return (bool)isOverlapping;
}
