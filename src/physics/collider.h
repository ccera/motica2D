#ifndef COLLIDER_H
#define COLLIDER_H

#include "chipmunk.h"
#include "gameobject.h"

class Collider
{
public:
    Collider();

    GameObject *gameObject;
};

#endif // COLLIDER_H
