#include "engine.h"

Engine::Engine()
{
    qDebug() << "Initializing engine...";
    isGLInitialized = false;
    next_model_id = 1;
    initDefaultSpriteMesh();
}

void Engine::addTexture(Texture *texture)
{
    this->arrTextures.push_back(texture);

    if(scene->isSceneGLPrepared) {
        scene->bindTexture(texture);
        texture->isGLBound = true;
    }
}

void Engine::addSprite(Sprite *sprite)
{
    sprite->mesh = this->getMesh("DEFAULT_SPRITE_MESH");
    sprite->model_id = next_model_id;
    next_model_id++;

    arrModels.push_back(sprite);
    this->arrSprites.push_back(sprite);
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

