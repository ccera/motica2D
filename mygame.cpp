//
//  Copyright (c) 2014 Emir Cerić, Dejan Omasta. All rights reserved.
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

#include "mygame.h"

MyGame::MyGame(Engine *engine) :
    GameObject()
{
    m_engine = engine;
    m_engine->addGameObject(this);

    prince = new Player(engine);

    planet = m_engine->newTexture(Resource("textures/Planet.png"));
    background = m_engine->newTexture(Resource("textures/Background.png"));
    bottle = m_engine->newTexture(Resource("textures/Bottle.png"));
    fish = m_engine->newTexture(Resource("textures/Fish.png"));


    sprBackground = m_engine->newSprite(background);
    sprBackground->setName("Background");
    sprBackground->setPosition(QVector3D(512,256,-10));
    sprBackground->setSize(QVector3D(1024,512,1));

    /*
    sprPlanet = m_engine->newSprite(planet);
    sprPlanet->setName("Planet");
    sprPlanet->transform->setPosition(800,320,10);
    sprPlanet->transform->setSize(512,512,0);

    sprBottle = m_engine->newSprite(bottle);
    sprBottle->setName("Bottle");
    sprBottle->transform->setPosition(500,400,11);
    sprBottle->transform->setSize(32,512,0);

    sprFish = m_engine->newSprite(fish);
    sprFish->setName("Fish");
    sprFish->transform->setPosition(300,300,99);
    sprFish->transform->setSize(128,256,0);

    */

    pod = m_engine->createPhysicsBodyBox(0,10000,50, PHYSICS_BODY_STATIC);
    pod->setPosition(QVector3D(500,0,0));

    podShape = m_engine->createPhysicsShapeBox(10000,50, QVector2D(0,0));
    podShape->setFriction(0.5f);
    m_engine->addShapeToBody(podShape, pod);
    m_engine->addShapeToSpace(podShape);


    PhysicsBody *platformaBody = m_engine->createPhysicsBodyBox(0,210,30, PHYSICS_BODY_STATIC);
    platformaBody->setPosition(QVector3D(200, 90, 0));
    PhysicsShape *platformShape = m_engine->createPhysicsShapeBox(210,30,QVector2D(0,0));
    platformShape->setFriction(0.5f);
    m_engine->addShapeToBody(platformShape, platformaBody);
    m_engine->addShapeToSpace(platformShape);

    PhysicsBody *platformaBody2 = m_engine->createPhysicsBodyBox(0,210,30, PHYSICS_BODY_STATIC);
    platformaBody2->setPosition(QVector3D(100, 30, 0));
    PhysicsShape *platformShape2 = m_engine->createPhysicsShapeBox(210,30,QVector2D(0,0));
    platformShape2->setFriction(0.5f);
    m_engine->addShapeToBody(platformShape2, platformaBody2);
    m_engine->addShapeToSpace(platformShape2);

    /*
    PhysicsBody *platformaBody3 = m_engine->createPhysicsBodyBox(0,210,30, PHYSICS_BODY_STATIC);
    platformaBody3->setPosition(QVector3D(450, 200, 0));
    PhysicsShape *platformShape3 = m_engine->createPhysicsShapeBox(210,30,QVector2D(0,0));
    platformShape3->setFriction(0.5f);
    m_engine->addShapeToBody(platformShape3, platformaBody3);
    m_engine->addShapeToSpace(platformShape3);
    */

    m_engine->physicsWorld->setGravity(0, -200.0f);
}

void MyGame::update(float dt)
{
    //if(sprFish->transform->x > 1000) sprFish->transform->setPosition(0, 300, 99);
    //sprFish->transform->translateFor(3,0,0);
}
