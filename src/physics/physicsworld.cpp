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

    //cpShape *shape = cpBoxShapeNew(body, width, height);
    //cpSpaceAddShape(space, shape);

    PhysicsBody *obj = new PhysicsBody(this);
    obj->bodyShape = PHYSICS_BODYSHAPE_BOX;
    obj->bodyState = bState;
    obj->body = body;
    //obj->shape = shape;
    //obj->shape->data = obj;
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
    //obj->shape = shape;
    //obj->shape->data = obj;
    obj->width = diametar;
    obj->height = diametar;
    obj->diameter = diametar;
    obj->mass = mass;

    return obj;
}

void PhysicsWorld::postSolve(cpArbiter *arb, cpSpace *space, void *ignore)
{
    CP_ARBITER_GET_SHAPES(arb, a, b);

    PhysicsBody *pA = (PhysicsBody*)cpShapeGetUserData(a);
    PhysicsBody *pB = (PhysicsBody*)cpShapeGetUserData(b);

    if(pA->parentGameObject != NULL) {
        pA->parentGameObject->collide(pB);
    }

    if(pB->parentGameObject != NULL) {
        pB->parentGameObject->collide(pA);
    }
}

PhysicsShape* PhysicsWorld::createPhysicsShapeBox(PhysicsBody *body, float width, float height, const QVector2D &offset)
{
    cpVect niz[] = {
        cpv(-(width/2.0f), -(height/2.0f)),
        cpv(-(width/2.0f),  (height/2.0f)),
        cpv( (width/2.0f),  (height/2.0f)),
        cpv( (width/2.0f),  -(height/2.0f))
    };

    //cpShape *shape = cpBoxShapeNew(NULL, width, height);
    cpShape *shape = cpPolyShapeNew(body->body, 4, niz, cpv(offset.x(), offset.y()));
    cpSpaceAddShape(space, shape);
    PhysicsShape *obj = new PhysicsShape(this);
    obj->shape = shape;
    obj->shapeType = PHYSICS_SHAPE_BOX;
    obj->width = width;
    obj->height = height;
    if(width > height) obj->diameter = width;
    else obj->diameter = height;
    obj->shape->data = obj;
}

PhysicsShape* PhysicsWorld::createPhysicsShapeCircle(PhysicsBody *body, float diametar, const QVector2D &offset)
{
    cpShape *shape = cpCircleShapeNew(body->body, diametar, cpv(offset.x(), offset.y()));
    cpSpaceAddShape(space, shape);
    PhysicsShape *obj = new PhysicsShape(this);
    obj->shape = shape;
    obj->shapeType = PHYSICS_SHAPE_CIRCLE;
    obj->width = diametar;
    obj->height = diametar;
    obj->diameter = diametar;
    obj->shape->data = obj;
}

//void PhysicsWorld::addShapeToBody(PhysicsShape *shape, PhysicsBody *body)
//{
//    cpShapeSetBody(shape->shape, body->body);
//}

void PhysicsWorld::addShapeToSpace(PhysicsShape *shape)
{
    cpSpaceAddShape(space, shape->shape);
}

//void PhysicsWorld::checkOverlapping(cpShape *shape, cpContactPointSet *points, void *data)
//{
//    if(shape->data != NULL) {
//        ((QList<PhysicsBody*>*)data)->append((PhysicsBody*)shape->data);
//    }
//}

//QList<PhysicsBody*> PhysicsWorld::checkForOverlappingObjects(PhysicsBody *obj)
//{
//    QList<PhysicsBody*> lista;
//    cpSpaceShapeQuery(space, obj->shape, checkOverlapping, (void*)&lista);
//    return lista;
//}

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
