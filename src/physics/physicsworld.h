//
//  Copyright (c) 2014 Emir Cerić. All rights reserved.
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

#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include <QObject>
#include "chipmunk.h"
#include "physicsbody.h"
#include "types.h"
#include "engine.h"
#include "physicsshape.h"

class PhysicsBody;
class PhysicsShape;

class PhysicsWorld : public QObject
{
    Q_OBJECT
public:
    explicit PhysicsWorld(QObject *parent = 0);

    void updateWorld(float dt);
    void setGravity(float x, float y);
    void setDamping(float d);

    PhysicsBody* createBoxBody(float mass, float width, float height, PhysicsBodyState bState = PHYSICS_BODY_SIMULATED);
    PhysicsBody* createCircleBody(float mass, float diametar, PhysicsBodyState bState = PHYSICS_BODY_SIMULATED);
    PhysicsShape* createPhysicsShapeBox(float width, float height, const QVector2D &offset);
    PhysicsShape* createPhysicsShapeCircle(float diametar, const QVector2D &offset);
    void addShapeToBody(PhysicsShape *shape, PhysicsBody *body);
    void addShapeToSpace(PhysicsShape *shape);

    cpSpace *space;
signals:

public slots:

private:
    static void postSolve(cpArbiter *arb, cpSpace *space, void *ignore);
};

#endif // PHYSICSWORLD_H
