#ifndef DATASTORE_H
#define DATASTORE_H

#include <QVector>
#include "sprite.h"
#include "animatedsprite.h"
#include "physicsworld.h"
#include "physicsobject.h"
#include "gameobject.h"
#include "scene.h"
#include "window.h"
#include "utils.h"
#include "mesh.h"
#include "model.h"
#include "types.h"

class Window;
class Scene;
class Sprite;
class AnimatedSprite;
class Texture;
class Model;
class PhysicsWorld;
class PhysicsObject;
class GameObject;

class Engine
{
public:
    Engine();

    Texture*        newTexture(const QString &file);
    void            addTexture(Texture *texture);
    Texture*        getTexture(const QString &name);
    Sprite*         newSprite(const QString &texture_name);
    Sprite*         newSprite(Texture *texture);
    void            addSprite(Sprite *sprite);
    AnimatedSprite* newAnimatedSprite(int rows, int columns, const QString &texture_name);
    AnimatedSprite* newAnimatedSprite(int rows, int columns, Texture *texture);
    void            addAnimatedSprite(AnimatedSprite *sprite);
    void            addMesh(Mesh *mesh);
    Mesh*           getMesh(const QString &name);
    void            addGameObject(GameObject *e);

    PhysicsObject* createPhysicsObjectBox(float mass, float width, float height, PhysicsBodyState bState = PHYSICSBODY_SIMULATED);
    PhysicsObject* createPhysicsObjectCircle(float mass, float diametar, PhysicsBodyState bState = PHYSICSBODY_SIMULATED);
    QList<PhysicsObject*> checkForOverlappingPhysicsObjects(PhysicsObject *obj);
    void            addPhysicsObject(PhysicsObject *object);

    void initDefaultSpriteMesh();

    QVector<Texture*> arrTextures;
    QVector<Sprite*> arrSprites;
    QVector<Model*> arrModels;
    QVector<AnimatedSprite*> arrAnimSprites;
    QVector<GameObject*> arrGameObjects;
    QVector<PhysicsObject*> arrPhysicsObjects;
    QVector<Mesh*> arrMeshes;

    Scene *scene;
    Window *engine;
    PhysicsWorld *physicsWorld;

    bool isGLInitialized;
    float camera_x;
    float camera_y;
    float camera_width;
    float camera_height;
    float camera_near;
    float camera_far;
    ViewportType viewport_type;

private:
    int next_model_id; // Id koji se dodjeljuje svakom modelu

};

#endif // DATASTORE_H
