#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QString>

class Settings
{
public:
    static bool DEBUG_PHYSICS_RENDER;
    static int  OPENGL_VERSION;

    static void checkGraphicsCapabilities();
};

#endif
