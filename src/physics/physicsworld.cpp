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

#include "physicsworld.h"

PhysicsWorld::PhysicsWorld(QObject *parent) :
    QObject(parent)
{
    space = cpSpaceNew();
    cpSpaceSetDefaultCollisionHandler(space, NULL, NULL, postSolve, NULL, NULL);
}

PhysicsBody* PhysicsWorld::createBoxBody(float mass, float width, float height, PhysicsBodyState bState)
{
    cpBody *body = 0;
    cpFloat i = 0;
    switch (bState) {
    case PHYSICS_BODY_SIMULATED:
        i = cpMomentForBox(mass, width, height);
        body = cpBodyNew(mass, i);
        cpSpaceAddBody(space, body);
        break;
    case PHYSICS_BODY_ROUGE:
        body = cpBodyNew(1, 1);
        break;
    case PHYSICS_BODY_STATIC:
        body = cpBodyNewStatic();
        break;
    default:
        break;
    }

    PhysicsBody *obj = new PhysicsBody(this);
    obj->bodyShape = PHYSICS_BODYSHAPE_BOX;
    obj->bodyState = bState;
    obj->body = body;
    obj->width = width;
    obj->height = height;
    if(width > height) obj->diameter = width;
    else obj->diameter = height;
    obj->mass = mass;

    return obj;
}

PhysicsBody* PhysicsWorld::createCircleBody(float mass, float diametar, PhysicsBodyState bState)
{
    cpBody *body = 0;
    cpFloat i = 0;
    switch (bState) {
    case PHYSICS_BODY_SIMULATED:
        i = cpMomentForCircle(mass, 0.0f, diametar, cpvzero);
        body = cpBodyNew(mass, i);
        cpSpaceAddBody(space, body);
        break;
    case PHYSICS_BODY_ROUGE:
        body = cpBodyNew(1, 1);
        break;
    case PHYSICS_BODY_STATIC:
        body = cpBodyNewStatic();
        break;
    default:
        break;
    }

    PhysicsBody *obj = new PhysicsBody(this);
    obj->bodyShape = PHYSICS_BODYSHAPE_CIRCLE;
    obj->bodyState = bState;
    obj->body = body;
    obj->width = diametar;
    obj->height = diametar;
    obj->diameter = diametar;
    obj->mass = mass;

    return obj;
}

void PhysicsWorld::postSolve(cpArbiter *arb, cpSpace *space, void *ignore)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);

    PhysicsShape *pA = (PhysicsShape*)cpShapeGetUserData(a);
    PhysicsShape *pB = (PhysicsShape*)cpShapeGetUserData(b);

    if(pA->parentGameObject != NULL) {
        pA->parentGameObject->collide(pB);
    }

    if(pB->parentGameObject != NULL) {
        pB->parentGameObject->collide(pA);
    }
}

PhysicsShape* PhysicsWorld::createPhysicsShapeBox(float width, float height, const QVector2D &offset)
{
    cpVect niz[] = {
        cpv(-(width/2.0f), -(height/2.0f)),
        cpv(-(width/2.0f),  (height/2.0f)),
        cpv( (width/2.0f),  (height/2.0f)),
        cpv( (width/2.0f),  -(height/2.0f))
    };

    cpShape *shape = cpPolyShapeNew(NULL, 4, niz, cpv(offset.x(), offset.y()));
    PhysicsShape *obj = new PhysicsShape(this);
    obj->shape = shape;
    obj->shapeType = PHYSICS_SHAPE_BOX;
    obj->width = width;
    obj->height = height;
    if(width > height) obj->diameter = width;
    else obj->diameter = height;
    obj->shape->data = obj;
    obj->offset = offset;

    return obj;
}

PhysicsShape* PhysicsWorld::createPhysicsShapeCircle(float diametar, const QVector2D &offset)
{
    cpShape *shape = cpCircleShapeNew(NULL, diametar, cpv(offset.x(), offset.y()));
    PhysicsShape *obj = new PhysicsShape(this);
    obj->shape = shape;
    obj->shapeType = PHYSICS_SHAPE_CIRCLE;
    obj->width = diametar;
    obj->height = diametar;
    obj->diameter = diametar;
    obj->shape->data = obj;
    obj->offset = offset;

    return obj;
}

void PhysicsWorld::addShapeToBody(PhysicsShape *shape, PhysicsBody *body)
{
    cpShapeSetBody(shape->shape, body->body);
}

void PhysicsWorld::addShapeToSpace(PhysicsShape *shape)
{
    cpSpaceAddShape(space, shape->shape);
}

void PhysicsWorld::updateWorld(float dt)
{
    cpSpaceStep(space, dt);
}

void PhysicsWorld::setGravity(float x, float y)
{
    cpSpaceSetGravity(space, cpv(x, y));
}

void PhysicsWorld::setDamping(float d)
{
    cpSpaceSetDamping(space, d);
}
