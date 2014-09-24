#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <QObject>
#include "chipmunk.h"
#include "physicsbody.h"
#include "types.h"
#include "engine.h"
#include "physicsshape.h"

class PhysicsBody;
class PhysicsShape;

class PhysicsWorld : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsWorld(QObject *parent = 0);

    void updateWorld(float dt);
    void setGravity(float x, float y);
    void setDamping(float d);

    PhysicsBody* createBoxBody(float mass, float width, float height, PhysicsBodyState bState = PHYSICS_BODY_SIMULATED);
    PhysicsBody* createCircleBody(float mass, float diametar, PhysicsBodyState bState = PHYSICS_BODY_SIMULATED);
    PhysicsShape* createPhysicsShapeBox(float width, float height, const QVector2D &offset);
    PhysicsShape* createPhysicsShapeCircle(float diametar, const QVector2D &offset);
    void addShapeToBody(PhysicsShape *shape, PhysicsBody *body);
    void addShapeToSpace(PhysicsShape *shape);

    cpSpace *space;
signals:

public slots:

private:
    static void postSolve(cpArbiter *arb, cpSpace *space, void *ignore);
};

#endif // PHYSICSWORLD_H
