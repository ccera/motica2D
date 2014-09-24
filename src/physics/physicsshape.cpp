#include "physicsshape.h"

PhysicsShape::PhysicsShape(PhysicsWorld *world)
{
    m_world = world;
    parentGameObject = 0;
    width = 0;
    height = 0;
    diameter = 0;
    shape = 0;
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
