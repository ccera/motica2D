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

class Window;
class Scene;
class Sprite;
class AnimatedSprite;
class Texture;
class Model;

class Engine
{
public:
    Engine();

    void  addMesh(Mesh *mesh);
    void  addTexture(Texture *texture);
    void  addSprite(Sprite *sprite);
    void  addGameObject(GameObject *e);
    void  addAnimatedSprite(AnimatedSprite *sprite);
    void  addPhysicsObject(PhysicsObject *object);
    Mesh* getMesh(const QString &name);
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
