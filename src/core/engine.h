#ifndef DATASTORE_H
#define DATASTORE_H

#include <QVector>
#include "sprite.h"
#include "animatedsprite.h"
#include "physicsworld.h"
#include "physicsbody.h"
#include "gameobject.h"
#include "scene.h"
#include "window.h"
#include "utils.h"
#include "mesh.h"
#include "model.h"
#include "types.h"
#include "physicsshape.h"

class Window;
class Scene;
class Sprite;
class AnimatedSprite;
class Texture;
class Model;
class PhysicsWorld;
class PhysicsBody;
class GameObject;
class PhysicsShape;

class Engine
{
public:
    Engine();

    // Graphics helper functions
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

    // Physics helper functions
    PhysicsBody*    newPhysicsBodyBox(float mass, float width, float height, PhysicsBodyState bState = PHYSICS_BODY_SIMULATED);
    PhysicsBody*    newPhysicsBodyCircle(float mass, float diametar, PhysicsBodyState bState = PHYSICS_BODY_SIMULATED);
    PhysicsShape*   newPhysicsShapeBox(float width, float height, const QVector2D &offset);
    PhysicsShape*   newPhysicsShapeCircle(float diametar, const QVector2D &offset);
    void            addShapeToBody(PhysicsShape *shape, PhysicsBody *body);
    void            addShapeToSpace(PhysicsShape *shape);


    QVector<Texture*>        arrTextures;
    QVector<Sprite*>         arrSprites;
    QVector<Model*>          arrModels;
    QVector<AnimatedSprite*> arrAnimSprites;
    QVector<GameObject*>     arrGameObjects;
    QVector<PhysicsBody*>    arrPhysicsBodies;
    QVector<PhysicsShape*>   arrPhysicsShapes;
    QVector<Mesh*>           arrMeshes;

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
    void initDefaultSpriteMesh();
};

#endif // DATASTORE_H
