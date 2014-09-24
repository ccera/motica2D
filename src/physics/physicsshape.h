#ifndef PHYSICSSHAPE_H
#define PHYSICSSHAPE_H

#include "physicsworld.h"
#include <QVector2D>
#include "utils.h"
#include "chipmunk.h"
#include "gameobject.h"
#include "types.h"
#include "transform.h"
#include "physicsbody.h"

class PhysicsWorld;
class GameObject;

class PhysicsShape
{
    friend class PhysicsWorld;

public:
    PhysicsShape(PhysicsWorld *world);

    QList<PhysicsShape*> listOverlappingObjects();
    bool        isOverlapping();
    void        setFriction(float f);

    cpShape *shape;
    PhysicsObjectShapeType shapeType;
    GameObject *parentGameObject;
    int userType;

private:
    static void checkOverlappingList(cpShape *shape, cpContactPointSet *points, void *data);
    static void checkOverlappingBool(cpShape *shape, cpContactPointSet *points, void *data);
    PhysicsWorld *m_world;
    float width;
    float height;
    float diameter;
};

#endif // PHYSICSSHAPE_H
