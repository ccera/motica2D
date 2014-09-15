#include "engine.h"

Engine::Engine()
{
    qDebug() << "Initializing engine...";
    isGLInitialized = false;
    next_model_id = 1;
    initDefaultSpriteMesh();
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

void Engine::addPhysicsObject(PhysicsObject *object)
{
    this->arrPhysicsObjects.push_back(object);
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

