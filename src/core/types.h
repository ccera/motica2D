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

#ifndef TYPES_H
#define TYPES_H

enum _ViewportType {
    VIEWPORT_PIXEL,
    VIEWPORT_STRECH_X,
    VIEWPORT_STRECH_Y,
    VIEWPORT_STRECH_XY,
};
typedef enum _ViewportType ViewportType;



enum _PhysicsObjectShapeType {
    PHYSICS_SHAPE_CIRCLE,
    PHYSICS_SHAPE_HOLOW_CIRCLE,
    PHYSICS_SHAPE_BOX,
    PHYSICS_SHAPE_POLYGON
};
typedef enum _PhysicsObjectShapeType PhysicsObjectShapeType;



enum _PhysicsObjectBodyShape {
    PHYSICS_BODYSHAPE_CIRCLE,
    PHYSICS_BODYSHAPE_BOX
};
typedef enum _PhysicsObjectBodyShape PhysicsObjectBodyShape;



enum _PhysicsBodyState {
    PHYSICS_BODY_STATIC,
    PHYSICS_BODY_SIMULATED,
    PHYSICS_BODY_ROUGE
};
typedef enum _PhysicsBodyState PhysicsBodyState;

#endif // TYPES_H
