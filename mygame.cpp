#include "mygame.h"

MyGame::MyGame(QObject *parent) :
    QObject(parent)
{
    game.setWindowSize(1024, 576);
    game.setCamera2DSize(1024,576);
    game.setAnimating(true);
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
    sprBackground.transform->setPosition(512,288,-10);
    sprBackground.transform->setSize(1024,576,0);
    game.addSprite(&sprBackground);

    sprPlanet.setTexture(&planet);
    sprPlanet.setName("Flames");
    sprPlanet.transform->setPosition(800,320,10);
    sprPlanet.transform->setSize(293,535,0);
    game.addSprite(&sprPlanet);

    sprBottle.setTexture(&bottle);
    sprBottle.setName("Bottle");
    sprBottle.transform->setPosition(500,400,11);
    sprBottle.transform->setSize(34,401,0);
    game.addSprite(&sprBottle);

    sprFish.setTexture(&fish);
    sprFish.setName("Fish");
    sprFish.transform->setPosition(10,300,12);
    sprFish.transform->setSize(153,393,0);
    game.addSprite(&sprFish);

    labela.text = "Motica2D";
    labela.fontSize = 30;
    labela.transform->setSize(200,50,0);
    labela.transform->setPosition(512,450,1);
    labela.color.setRgb(255,255,255);
    game.addLabel(&labela);

    animation.setTargetObject(&sprFish);
    animation.setPropertyName("position");
    animation.setDuration(8000);
    animation.setStartValue(QVector3D(10,300,12));
    animation.setEndValue(QVector3D(800,300,12));
    animation.setEasingCurve(QEasingCurve::Linear);
    animation.setLoopCount(-1);
    //animation.start();
    //animation.stop();
    //animation.setCurrentTime(0);
    animTm = 0;

    game.connectToEvents(this);
    connect(&game, SIGNAL(update(float)), this, SLOT(updateSlot(float)));
}

void MyGame::update(float dt)
{
    animTm += dt;
//    qDebug() << dt;
//    sprFish.transform->translateFor(dt/2.0f,0,0);
    //animation.setCurrentTime((int)animTm);
    //animation.resume();
}

void MyGame::updateSlot(float dt)
{
    sprFish.transform->translateFor(dt/10,0,0);
}
