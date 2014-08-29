#include "mygame.h"

MyGame::MyGame(MoEngine *engine) :
    GameObject()
{
    m_engine = engine;
    m_engine->addGameObject(this);

    prince = new Player(engine);

    planet.setFile(Resource("textures/Planet.png"));
    m_engine->addTexture(&planet);

    background.setFile(Resource("textures/Background.png"));
    m_engine->addTexture(&background);

    bottle.setFile(Resource("textures/Bottle.png"));
    engine->addTexture(&bottle);

    fish.setFile(Resource("textures/Fish.png"));
    m_engine->addTexture(&fish);

    sprBackground.setTexture(&background);
    sprBackground.setName("Background");
    sprBackground.transform->setPosition(512,256,-10);
    sprBackground.transform->setSize(1024,512,0);
    m_engine->addSprite(&sprBackground);

    sprPlanet.setTexture(&planet);
    sprPlanet.setName("Planet");
    sprPlanet.transform->setPosition(800,320,10);
    sprPlanet.transform->setSize(512,512,0);
    m_engine->addSprite(&sprPlanet);

    sprBottle.setTexture(&bottle);
    sprBottle.setName("Bottle");
    sprBottle.transform->setPosition(500,400,11);
    sprBottle.transform->setSize(32,512,0);
    m_engine->addSprite(&sprBottle);

    sprFish.setTexture(&fish);
    sprFish.setName("Fish");
    sprFish.transform->setPosition(300,300,99);
    sprFish.transform->setSize(128,256,0);
    m_engine->addSprite(&sprFish);
/*
    for(int n=0; n < 100; n++) {
        sprNiz[n].setTexture(&fish);
        float x = qrand() % 1024;
        float y = qrand() % 512;
        sprNiz[n].transform->setPosition(x,y,(float)20.f+(n/10.0f));
        sprNiz[n].transform->setSize(128,256,0);
        m_engine->addSprite(&sprNiz[n]);
    }
*/
    m_engine->showWindow();
}

void MyGame::update(float dt)
{
    if(sprFish.transform->x > 1000) sprFish.transform->setPosition(0, 300, 99);
    sprFish.transform->translateFor(3,0,0);

    for(int n=0; n < 100; n++) {
        sprNiz[n].transform->rotateFor(0,0,1);
    }
}
