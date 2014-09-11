#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <QObject>
#include "chipmunk.h"
#include "physicsobject.h"
#include "physicstypes.h"

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

    cpSpace *space;
signals:

public slots:

private:
    static void postSolve(cpArbiter *arb, cpSpace *space, void *ignore);



};

#endif // PHYSICSWORLD_H
