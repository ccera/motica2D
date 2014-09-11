#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <QObject>
#include "chipmunk.h"

enum RigidBodyType {
    RIGIDBODY_CIRCLE,
    RIGIDBODY_HOLOW_CIRCLE,
    RIGIDBODY_BOX,
    RIGIDBODY_POLYGON
};

class PhysicsWorld : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsWorld(QObject *parent = 0);

    void updateWorld(float dt);
    void setGravity(float x, float y);
    void setDamping(float d);

signals:

public slots:

private:
    cpSpace *space;

};

#endif // PHYSICSWORLD_H
