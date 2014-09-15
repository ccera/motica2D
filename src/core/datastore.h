#ifndef DATASTORE_H
#define DATASTORE_H

#include <QVector>
#include "sprite.h"
#include "animatedsprite.h"
#include "physicsworld.h"
#include "physicsobject.h"
#include "gameobject.h"
#include "scene.h"
#include "moengine.h"

class MoEngine;

class DataStore
{
public:
    DataStore();

    QVector<Sprite*> arrSprites;
    QVector<AnimatedSprite*> arrAnimSprites;
    QVector<GameObject*> arrGameObjects;
    QVector<PhysicsObject*> arrPhysicsObjects;

    Scene *scene;
    MoEngine *engine;
    PhysicsWorld *physicsWorld;
};


Q_GLOBAL_STATIC(DataStore, dataStore)

#endif // DATASTORE_H
