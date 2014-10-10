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

#include "engine.h"

Engine::Engine(QObject *parent)
       : QObject(parent)
{
    qDebug() << "Initializing engine...";
    Settings::checkGraphicsCapabilities();
    isGLInitialized = false;
    next_model_id = 1;
    initDefaultSpriteMesh();
    window = 0;
}

Texture* Engine::newTexture(const QString &file)
{
    Texture *tex = new Texture(file);
    this->addTexture(tex);
    return tex;
}

void Engine::addTexture(Texture *texture)
{
    this->arrTextures.push_back(texture);

    if(scene->isSceneGLPrepared) {
        scene->bindTexture(texture);
        texture->isGLBound = true;
    }
}

Sprite* Engine::newSprite(const QString &texture_name)
{
    Sprite *spr = new Sprite(this->getTexture(texture_name));
    this->addSprite(spr);
    return spr;
}

Sprite* Engine::newSprite(Texture *texture)
{
    Sprite *spr = new Sprite(texture);
    this->addSprite(spr);
    return spr;
}

void Engine::addSprite(Sprite *sprite)
{
    sprite->mesh = this->getMesh("DEFAULT_SPRITE_MESH");
    sprite->model_id = next_model_id;
    next_model_id++;

    arrModels.push_back(sprite);
    this->arrSprites.push_back(sprite);
}


AnimatedSprite* Engine::newAnimatedSprite(int rows, int columns, const QString &texture_name)
{
    AnimatedSprite *as = new AnimatedSprite(rows, columns, this->getTexture(texture_name));
    this->addAnimatedSprite(as);
    return as;
}

AnimatedSprite* Engine::newAnimatedSprite(int rows, int columns, Texture *texture)
{
    AnimatedSprite *as = new AnimatedSprite(rows, columns, texture);
    this->addAnimatedSprite(as);
    return as;
}

void Engine::addAnimatedSprite(AnimatedSprite *sprite)
{
    sprite->mesh = this->getMesh("DEFAULT_SPRITE_MESH");
    sprite->model_id = next_model_id;
    next_model_id++;

    arrModels.push_back(sprite);
    this->arrAnimSprites.push_back(sprite);
}

void Engine::addGameObject(GameObject *e)
{
    this->arrGameObjects.push_back(e);
}

PhysicsBody*  Engine::newPhysicsBodyBox(float mass, float width, float height, PhysicsBodyState bState)
{
    PhysicsBody *po = physicsWorld->createBoxBody(mass, width, height, bState);
    this->arrPhysicsBodies.push_back(po);
    return po;
}

PhysicsBody*  Engine::newPhysicsBodyCircle(float mass, float diametar, PhysicsBodyState bState)
{
    PhysicsBody *po = physicsWorld->createCircleBody(mass, diametar, bState);
    this->arrPhysicsBodies.push_back(po);
    return po;
}

PhysicsShape* Engine::newPhysicsShapeBox(float width, float height, const QVector2D &offset)
{
    PhysicsShape *ps = physicsWorld->createPhysicsShapeBox(width, height, offset);
    this->arrPhysicsShapes.push_back(ps);
    return ps;
}

PhysicsShape* Engine::newPhysicsShapeCircle(float diametar, const QVector2D &offset)
{
    PhysicsShape *ps = physicsWorld->createPhysicsShapeCircle(diametar, offset);
    this->arrPhysicsShapes.push_back(ps);
    return ps;
}

void Engine::addShapeToBody(PhysicsShape *shape, PhysicsBody *body)
{
    physicsWorld->addShapeToBody(shape, body);
}

void Engine::addShapeToSpace(PhysicsShape *shape)
{
    physicsWorld->addShapeToSpace(shape);
}

void Engine::addMesh(Mesh *mesh)
{
    arrMeshes.push_back(mesh);

    if(scene->isSceneGLPrepared) {
        scene->bindMesh(mesh);
        mesh->isGLBound = true;
    }
}

Mesh* Engine::getMesh(const QString &name)
{
    for(int n=0; n < arrMeshes.size(); n++) {
        if(arrMeshes[n]->name.compare(name) == 0) {
            return arrMeshes[n];
        }
    }
    return 0;
}

Texture* Engine::getTexture(const QString &name)
{
    for(int n=0; n < arrTextures.size(); n++) {
        if(arrTextures[n]->name.compare(name) == 0) {
            return arrTextures[n];
        }
    }
    return 0;
}

void Engine::initDefaultSpriteMesh()
{
    float vdata[] = {   0.0,0.0,  0.0,0.0,1.0,  -0.5,-0.5,0.0,
                        1.0,0.0,  0.0,0.0,1.0,   0.5,-0.5,0.0,
                        1.0,1.0,  0.0,0.0,1.0,   0.5, 0.5,0.0,
                        0.0,1.0,  0.0,0.0,1.0,  -0.5, 0.5,0.0
    };
    int idata[] = { 0,1,2,0,2,3, 0,2,1,0,3,2 };
    arrMeshes.push_back(new Mesh(vdata, idata, 8, 4, "DEFAULT_SPRITE_MESH"));
}

void Engine::createWindow(float width, float height)
{
    window = new Window(this);
    window->setWindowSize(width, height);
    window->setCamera2DSize(width, height);
    window->setAnimating(true);
    window->setViewport2DType(VIEWPORT_STRECH_XY);
    window->setBackgroundColor(0.5f, 0.0f, 0.3f, 1.0f);

}

void Engine::createWindow(float width, float height, float camera_width, float camera_height, ViewportType type)
{
    window = new Window(this);
    window->setWindowSize(width, height);
    window->setCamera2DSize(camera_width, camera_height);
    window->setAnimating(true);
    window->setViewport2DType(type);
    window->setBackgroundColor(0.5f, 0.0f, 0.3f, 1.0f);
}

void Engine::showWindow()
{
    window->showWindow();
}


