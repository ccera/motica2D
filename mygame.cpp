#include "mygame.h"

MyGame::MyGame(QObject *parent) :
    QObject(parent)
{
    game.setWindowSize(1024, 500);
    game.setAnimating(true);
    game.setBackgroundColor(0.5f, 0.0f, 0.3f, 1.0f);
    game.showWindow();

    tex.setFile(Resource("textures/Crate.png"));
    game.addTexture(&tex);

    background.setFile(Resource("textures/Bananaz.jpg"));
    game.addTexture(&background);

    sprBackground.setTexture(&background);
    sprBackground.transform->setPosition(512,250,-10.0f);
    sprBackground.transform->setSize(1024,500,0);
    game.addSprite(&sprBackground);

    sprCrate.setTexture(&tex);
    sprCrate.transform->setPosition(300,300,10.0f);
    sprCrate.transform->setSize(100,100,0);
    game.addSprite(&sprCrate);

    labela.text = "Motica2D";
    labela.fontSize = 30;
    labela.transform->setSize(200,50,0);
    labela.transform->setPosition(512,450,1);
    labela.color.setRgb(255,255,255);
    game.addLabel(&labela);

    connect(&game, SIGNAL(update(float)), this, SLOT(update(float)));
}

void MyGame::update(float dt)
{
    sprCrate.transform->rotateFor(0,0,3.0f);
}
