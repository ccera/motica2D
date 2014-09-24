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
