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

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "physicsworld.h"
#include <QVector2D>
#include "utils.h"
#include "chipmunk.h"
#include "gameobject.h"
#include "types.h"
#include "transform.h"

class PhysicsWorld;
class GameObject;

class PhysicsBody
{
    friend class PhysicsWorld;

public:
    PhysicsBody(PhysicsWorld *world);

    void setPosition(const QVector3D &position);
    void setRotation(const QVector3D &rotation);
    QVector3D getPosition();
    QVector3D getRotation();

    void        applyForce(float x, float y);
    void        applyForceWithOffset(float x, float y, float offset_x, float offset_y);
    void        applyImpulse(float x, float y);
    void        applyImpulseWithOffset(float x, float y, float offset_x, float offset_y);
    void        setMaxVelocity(float max);
    QVector2D   getVelocity();
    void        setVelocity(float x, float y);
    float       getWidth();
    float       getHeight();
    float       getDiameter();
    void        disableRotation();
    void        enableRotation();
    void        setMomentOfInertia(float i);

    cpBody *body;
    PhysicsObjectBodyShape bodyShape;
    PhysicsBodyState bodyState;

    QString name;

private:
    PhysicsWorld *m_world;
    float width;
    float height;
    float diameter;
    float mass;

};

#endif // RIGIDBODY_H
