//
//  Copyright (c) 2014 Emir Cerić. All rights reserved.
//
//  This file is part of Motica2D.
//
//  Motica2D is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Motica2D is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Motica2D.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef DATASTORE_H
#define DATASTORE_H

#include <QObject>
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

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);


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
    Window *window;
    PhysicsWorld *physicsWorld;

    bool isGLInitialized;
    float camera_x;
    float camera_y;
    float camera_width;
    float camera_height;
    float camera_near;
    float camera_far;
    ViewportType viewport_type;

signals:


public slots:
    // Window helper
    void createWindow(float width, float height);
    void createWindow(float width, float height, float camera_width, float camera_height, ViewportType type);
    void showWindow();


private:
    int next_model_id; // Id koji se dodjeljuje svakom modelu
    void initDefaultSpriteMesh();
};

#endif // DATASTORE_H
