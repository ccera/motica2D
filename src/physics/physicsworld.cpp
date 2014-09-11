#include "physicsworld.h"

PhysicsWorld::PhysicsWorld(QObject *parent) :
    QObject(parent)
{
    space = cpSpaceNew();
}

void PhysicsWorld::updateWorld(float dt)
{
    cpSpaceStep(space, dt);
}

void PhysicsWorld::setGravity(float x, float y)
{
    space->gravity = cpv(x, y);
}

void PhysicsWorld::setDamping(float d)
{
    cpSpaceSetDamping(space, d);
}
