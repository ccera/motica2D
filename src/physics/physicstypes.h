#ifndef PHYSICSTYPES_H
#define PHYSICSTYPES_H

enum _PhysicsObjectType {
    PHYSICSOBJECT_CIRCLE,
    PHYSICSOBJECT_HOLOW_CIRCLE,
    PHYSICSOBJECT_BOX,
    PHYSICSOBJECT_POLYGON
};
typedef enum _PhysicsObjectType PhysicsObjectType;

enum _PhysicsBodyState {
    PHYSICSBODY_STATIC,
    PHYSICSBODY_SIMULATED,
    PHYSICSBODY_ROUGE
};
typedef enum _PhysicsBodyState PhysicsBodyState;


#endif // PHYSICSTYPES_H