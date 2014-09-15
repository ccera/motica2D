#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <QObject>
#include "chipmunk.h"
#include "physicsobject.h"
#include "types.h"
#include "engine.h"

class PhysicsObject;

class PhysicsWorld : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsWorld(QObject *parent = 0);

    void updateWorld(float dt);
    void setGravity(float x, float y);
    void setDamping(float d);

    PhysicsObject* createBox(float mass, float width, float height, PhysicsBodyState bState = PHYSICSBODY_SIMULATED);
    PhysicsObject* createCircle(float mass, float diametar, PhysicsBodyState bState = PHYSICSBODY_SIMULATED);

    QList<PhysicsObject*> checkForOverlappingObjects(PhysicsObject *obj);

    cpSpace *space;
signals:

public slots:

private:
    static void postSolve(cpArbiter *arb, cpSpace *space, void *ignore);
    static void checkOverlapping(cpShape *shape, cpContactPointSet *points, void *data);




};

#endif // PHYSICSWORLD_H
