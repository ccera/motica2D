#include "mygame.h"

MyGame::MyGame(QObject *parent) :
    QObject(parent)
{
    game.setWindowSize(1024, 512);
    game.setCamera2DSize(1024,512);
    game.setAnimating(true);
    game.setViewport2DType(VIEWPORT_STRECH_XY);
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
    sprFish.transform->setPosition(300,300,12);
    sprFish.transform->setSize(128,256,0);
    game.addSprite(&sprFish);

    labela.text = "Motica2D";
    labela.fontSize = 30;
    labela.transform->setSize(200,50,0);
    labela.transform->setPosition(512,450,1);
    labela.color.setRgb(255,255,255);
    game.addLabel(&labela);

    animation.setTargetObject(&sprFish);
    animation.setPropertyName("position");
    animation.setDuration(4000);
    animation.setStartValue(QVector3D(10,400,12));
    animation.setEndValue(QVector3D(800,400,12));
    animation.setEasingCurve(QEasingCurve::CosineCurve);
    animation.setLoopCount(-1);
    animation.start();
    animation.pause();
    //animation.setCurrentTime(0);
    animTm = 0;



    game.connectToEvents(this);
    connect(&game, SIGNAL(update(float)), this, SLOT(updateSlot(float)));
}

void MyGame::update(float dt)
{
    //qDebug() << dt;
    animTm += dt;
//    if(sprFish.transform->x > 1000)
//        sprFish.transform->setPosition(0, 400, 0);
//    sprFish.transform->translateFor((int)dt/3,0,0);
    animation.setCurrentTime(animTm);
    //animation.setCurrentTime(0);
}

void MyGame::updateSlot(float dt)
{
    //sprFish.transform->translateFor((int)dt/3,0,0);
}
