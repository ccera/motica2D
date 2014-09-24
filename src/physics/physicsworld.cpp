#include "physicsworld.h"

PhysicsWorld::PhysicsWorld(QObject *parent) :
    QObject(parent)
{
    space = cpSpaceNew();
    cpSpaceSetDefaultCollisionHandler(space, NULL, NULL, postSolve, NULL, NULL);
}

PhysicsObject* PhysicsWorld::createBox(float mass, float width, float height, PhysicsBodyState bState)
{
    cpBody *body = 0;
    cpFloat i = 0;
    switch (bState) {
    case PHYSICSBODY_SIMULATED:
        i = cpMomentForBox(mass, width, height);
        body = cpBodyNew(mass, i);
        cpSpaceAddBody(space, body);
        break;
    case PHYSICSBODY_ROUGE:
        body = cpBodyNew(1, 1);
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
    obj->shapeType = PHYSICSOBJECT_BOX;
    obj->bodyState = bState;
    obj->body = body;
    obj->shape = shape;
    obj->shape->data = obj;
    obj->width = width;
    obj->height = height;
    if(width > height) obj->diameter = width;
    else obj->diameter = height;
    obj->mass = mass;
    obj->setSize(QVector3D(width, height, 1.0f));

    return obj;
}

PhysicsObject* PhysicsWorld::createCircle(float mass, float diametar, PhysicsBodyState bState)
{
    cpBody *body = 0;
    cpFloat i = 0;
    switch (bState) {
    case PHYSICSBODY_SIMULATED:
        i = cpMomentForCircle(mass, 0.0f, diametar, cpvzero);
        body = cpBodyNew(mass, i);
        cpSpaceAddBody(space, body);
        break;
    case PHYSICSBODY_ROUGE:
        body = cpBodyNew(1, 1);
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
    obj->shapeType = PHYSICSOBJECT_CIRCLE;
    obj->bodyState = bState;
    obj->body = body;
    obj->shape = shape;
    obj->shape->data = obj;
    obj->width = diametar;
    obj->height = diametar;
    obj->diameter = diametar;
    obj->mass = mass;
    obj->setSize(QVector3D(diametar, diametar, 1.0f));

    return obj;
}

void PhysicsWorld::postSolve(cpArbiter *arb, cpSpace *space, void *ignore)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);

    PhysicsObject *pA = (PhysicsObject*)cpShapeGetUserData(a);
    PhysicsObject *pB = (PhysicsObject*)cpShapeGetUserData(b);

    if(pA->parentGameObject != NULL) {
        pA->parentGameObject->collide(pB);
    }

    if(pB->parentGameObject != NULL) {
        pB->parentGameObject->collide(pA);
    }
}

void PhysicsWorld::checkOverlapping(cpShape *shape, cpContactPointSet *points, void *data)
{
    if(shape->data != NULL) {
        ((QList<PhysicsObject*>*)data)->append((PhysicsObject*)shape->data);
    }
}

QList<PhysicsObject*> PhysicsWorld::checkForOverlappingObjects(PhysicsObject *obj)
{
    QList<PhysicsObject*> lista;
    cpSpaceShapeQuery(space, obj->shape, checkOverlapping, (void*)&lista);
    return lista;
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
