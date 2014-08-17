#include "mygame.h"

MyGame::MyGame(QObject *parent) :
    QObject(parent)
{
    game.setWindowSize(1024, 512);
    game.setCamera2DSize(1024,512);
    game.setAnimating(true);
    game.setViewport2DType(VIEWPORT_PIXEL);
    game.setBackgroundColor(0.5f, 0.0f, 0.3f, 1.0f);
    game.showWindow();

    planet.setFile(Resource("textures/Planet.png"));
    game.addTexture(&planet);

    background.setFile(Resource("textures/Background.png"));
    game.addTexture(&background);

    bottle.setFile(Resource("textures/Bottle.png"));
    game.addTexture(&bottle);

    fish.setFile(Resource("textures/Fish.png"));
    game.addTexture(&fish);

    sprBackground.setTexture(&background);
    sprBackground.setName("Background");
    sprBackground.transform->setPosition(512,256,-10);
    sprBackground.transform->setSize(1024,512,0);
    game.addSprite(&sprBackground);

    sprPlanet.setTexture(&planet);
    sprPlanet.setName("Planet");
    sprPlanet.transform->setPosition(800,320,10);
    sprPlanet.transform->setSize(512,512,0);
    game.addSprite(&sprPlanet);

    sprBottle.setTexture(&bottle);
    sprBottle.setName("Bottle");
    sprBottle.transform->setPosition(500,400,11);
    sprBottle.transform->setSize(32,512,0);
    game.addSprite(&sprBottle);

    sprFish.setTexture(&fish);
    sprFish.setName("Fish");
    sprFish.transform->setPosition(300,300,99);
    sprFish.transform->setSize(128,256,0);
    game.addSprite(&sprFish);

    /*for(int n=0; n < 100; n++) {
        sprNiz[n].setTexture(&fish);
        float x = qrand() % 1024;
        float y = qrand() % 512;
        sprNiz[n].transform->setPosition(x,y,20+(n/2));
        sprNiz[n].transform->setSize(128,256,0);
        game.addSprite(&sprNiz[n]);
    }*/

    game.connectToEvents(this);
    connect(&game, SIGNAL(update(float)), this, SLOT(updateSlot(float)));
}

void MyGame::update(float dt)
{
    if(sprFish.transform->x > 1000) sprFish.transform->setPosition(0, 300, 99);
    sprFish.transform->translateFor(3,0,0);

}

void MyGame::updateSlot(float dt)
{
    //sprFish.transform->translateFor((int)dt/3,0,0);
}
