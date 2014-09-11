#include "physicsworld.h"

PhysicsWorld::PhysicsWorld(QObject *parent) :
    QObject(parent)
{
    space = cpSpaceNew();
}

PhysicsObject* PhysicsWorld::createBox(float mass, float width, float height, PhysicsBodyState bState)
{
    cpBody *body = 0;
    cpFloat i = 0;
    switch (bState) {
    case PHYSICSBODY_SIMULATED:
    case PHYSICSBODY_ROUGE:
        i = cpMomentForBox(mass, width, height);
        body = cpBodyNew(mass, i);
        cpSpaceAddBody(space, body);
        break;
    case PHYSICSBODY_STATIC:
        body = cpBodyNewStatic();
        break;
    default:
        break;
    }

    cpShape *shape = cpBoxShapeNew(body, width, height);
    cpSpaceAddShape(space, shape);

    PhysicsObject *obj = new PhysicsObject(this);
    obj->type = PHYSICSOBJECT_BOX;
    obj->bodyState = bState;
    obj->body = body;
    obj->shape = shape;

    return obj;
}

PhysicsObject* PhysicsWorld::createCircle(float mass, float diametar, PhysicsBodyState bState)
{
    cpBody *body = 0;
    cpFloat i = 0;
    switch (bState) {
    case PHYSICSBODY_SIMULATED:
    case PHYSICSBODY_ROUGE:
        i = cpMomentForCircle(mass, 0.0f, diametar, cpvzero);
        body = cpBodyNew(mass, i);
        cpSpaceAddBody(space, body);
        break;
    case PHYSICSBODY_STATIC:
        body = cpBodyNewStatic();
        break;
    default:
        break;
    }

    cpShape *shape = cpCircleShapeNew(body, diametar,cpvzero);
    cpSpaceAddShape(space, shape);

    PhysicsObject *obj = new PhysicsObject(this);
    obj->type = PHYSICSOBJECT_CIRCLE;
    obj->bodyState = bState;
    obj->body = body;
    obj->shape = shape;

    return obj;
}

void PhysicsWorld::postSolve(cpArbiter *arb, cpSpace *space, void *ignore)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);
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
