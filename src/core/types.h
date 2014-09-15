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
    PHYSICSOBJECT_CIRCLE,
    PHYSICSOBJECT_HOLOW_CIRCLE,
    PHYSICSOBJECT_BOX,
    PHYSICSOBJECT_POLYGON
};
typedef enum _PhysicsObjectShapeType PhysicsObjectShapeType;

enum _PhysicsBodyState {
    PHYSICSBODY_STATIC,
    PHYSICSBODY_SIMULATED,
    PHYSICSBODY_ROUGE
};
typedef enum _PhysicsBodyState PhysicsBodyState;

#endif // TYPES_H
