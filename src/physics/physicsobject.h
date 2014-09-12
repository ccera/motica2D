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
#include "physicstypes.h"
#include <QVector2D>
#include "utils.h"
#include "chipmunk.h"

class PhysicsWorld;

class PhysicsObject
{
public:
    PhysicsObject(PhysicsWorld *world);

    void        setPosition(float x, float y);
    QVector2D   getPosition();
    void        setRotation(float deg);
    float       getRotation();
    void        applyForce(float x, float y);
    void        applyForceWithOffset(float x, float y, float offset_x, float offset_y);
    void        applyImpulse(float x, float y);
    void        applyImpulseWithOffset(float x, float y, float offset_x, float offset_y);
    void        setFriction(float f);
    void        setMaxVelocity(float max);
    QVector2D   getVelocity();
    void        setVelocity(float x, float y);

    cpBody *body;
    cpShape *shape;
    PhysicsObjectType type;
    PhysicsBodyState bodyState;

private:
    PhysicsWorld *m_world;
};

#endif // RIGIDBODY_H
