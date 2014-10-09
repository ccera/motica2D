
QT += gui opengl openglextensions

QMAKE_CFLAGS_RELEASE += -std=c99
QMAKE_CFLAGS_DEBUG += -std=c99

INCLUDEPATH +=  \
    src/rendering \
    src/physics \
    src/physics/chipmunk \
    src/core \
    src/

HEADERS += \
    src/rendering/texture.h \
    src/rendering/sprite.h \
    src/rendering/utils.h \
    src/rendering/mesh.h \
    src/rendering/model.h \
    src/rendering/scene.h \
    src/rendering/transform.h \
    src/rendering/timer.h \
    mygame.h \
    src/core/gameobject.h \
    src/physics/chipmunk/chipmunk.h \
    src/core/mocolor.h \
    src/rendering/animatedsprite.h \
    src/core/keyboard.h \
    player.h \
    src/physics/physicsworld.h \
    src/core/window.h \
    src/core/engine.h \
    src/core/types.h \
    src/physics/physicsbody.h \
    src/physics/physicsshape.h \
    src/core/settings.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp \
    src/rendering/texture.cpp \
    src/rendering/sprite.cpp \
    src/rendering/utils.cpp \
    src/rendering/mesh.cpp \
    src/rendering/model.cpp \
    src/rendering/scene.cpp \
    src/rendering/transform.cpp \
    src/rendering/timer.cpp \
    mygame.cpp \
    src/core/gameobject.cpp \
    src/physics/chipmunk/chipmunk.c \
    src/physics/chipmunk/constraints/cpConstraint.c \
    src/physics/chipmunk/constraints/cpDampedRotarySpring.c \
    src/physics/chipmunk/constraints/cpDampedSpring.c \
    src/physics/chipmunk/constraints/cpGearJoint.c \
    src/physics/chipmunk/constraints/cpGrooveJoint.c \
    src/physics/chipmunk/constraints/cpPinJoint.c \
    src/physics/chipmunk/constraints/cpPivotJoint.c \
    src/physics/chipmunk/constraints/cpRatchetJoint.c \
    src/physics/chipmunk/constraints/cpRotaryLimitJoint.c \
    src/physics/chipmunk/constraints/cpSimpleMotor.c \
    src/physics/chipmunk/constraints/cpSlideJoint.c \
    src/physics/chipmunk/cpArbiter.c \
    src/physics/chipmunk/cpArray.c \
    src/physics/chipmunk/cpBB.c \
    src/physics/chipmunk/cpBBTree.c \
    src/physics/chipmunk/cpBody.c \
    src/physics/chipmunk/cpCollision.c \
    src/physics/chipmunk/cpHashSet.c \
    src/physics/chipmunk/cpPolyShape.c \
    src/physics/chipmunk/cpShape.c \
    src/physics/chipmunk/cpSpace.c \
    src/physics/chipmunk/cpSpaceComponent.c \
    src/physics/chipmunk/cpSpaceHash.c \
    src/physics/chipmunk/cpSpaceQuery.c \
    src/physics/chipmunk/cpSpaceStep.c \
    src/physics/chipmunk/cpSpatialIndex.c \
    src/physics/chipmunk/cpSweep1D.c \
    src/physics/chipmunk/cpVect.c \
    src/core/mocolor.cpp \
    src/rendering/animatedsprite.cpp \
    src/core/keyboard.cpp \
    player.cpp \
    src/physics/physicsworld.cpp \
    src/core/window.cpp \
    src/core/engine.cpp \
    src/physics/physicsbody.cpp \
    src/physics/physicsshape.cpp \
    src/core/settings.cpp

# Ovdje dodati sve što se treba koirati uz app
resources_dir.source = resources
resources_dir.target = Resursi

shaders_150_dir.source = src/rendering/Shaders_150
shaders_150_dir.target = Shaders

shaders_ES_dir.source = src/rendering/Shaders_ES
shaders_ES_dir.target = Shaders

shaders_120_dir.source = src/rendering/Shaders_120
shaders_120_dir.target = Shaders

DEPLOYMENTFOLDERS += resources_dir shaders_ES_dir shaders_150_dir shaders_120_dir

# Ovo dole je kopirano iz pro fajla kojeg kreira qtcreator ya qmlviewer aplikaciju. Bez ovoga ne deploya resurse.
defineTest(qtcAddDeployment) {
for(deploymentfolder, DEPLOYMENTFOLDERS) {
    item = item$${deploymentfolder}
    greaterThan(QT_MAJOR_VERSION, 4) {
        itemsources = $${item}.files
    } else {
        itemsources = $${item}.sources
    }
    $$itemsources = $$eval($${deploymentfolder}.source)
    itempath = $${item}.path
    $$itempath= $$eval($${deploymentfolder}.target)
    export($$itemsources)
    export($$itempath)
    DEPLOYMENT += $$item
}

MAINPROFILEPWD = $$PWD

android-no-sdk {
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        item = item$${deploymentfolder}
        itemfiles = $${item}.files
        $$itemfiles = $$eval($${deploymentfolder}.source)
        itempath = $${item}.path
        $$itempath = /data/user/qt/$$eval($${deploymentfolder}.target)
        export($$itemfiles)
        export($$itempath)
        INSTALLS += $$item
    }

    target.path = /data/user/qt

    export(target.path)
    INSTALLS += target
} else:android {
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        item = item$${deploymentfolder}
        itemfiles = $${item}.files
        $$itemfiles = $$eval($${deploymentfolder}.source)
        itempath = $${item}.path
        $$itempath = /assets/$$eval($${deploymentfolder}.target)
        export($$itemfiles)
        export($$itempath)
        INSTALLS += $$item
    }

    x86 {
        target.path = /libs/x86
    } else: armeabi-v7a {
        target.path = /libs/armeabi-v7a
    } else {
        target.path = /libs/armeabi
    }

    export(target.path)
    INSTALLS += target
} else:win32 {
    copyCommand =
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.source)
        source = $$replace(source, /, \\)
        sourcePathSegments = $$split(source, \\)
        target = $$OUT_PWD/$$eval($${deploymentfolder}.target)/$$last(sourcePathSegments)
        target = $$replace(target, /, \\)
        target ~= s,\\\\\\.?\\\\,\\,
        !isEqual(source,$$target) {
            !isEmpty(copyCommand):copyCommand += &&
            isEqual(QMAKE_DIR_SEP, \\) {
                copyCommand += $(COPY_DIR) \"$$source\" \"$$target\"
            } else {
                source = $$replace(source, \\\\, /)
                target = $$OUT_PWD/$$eval($${deploymentfolder}.target)
                target = $$replace(target, \\\\, /)
                copyCommand += test -d \"$$target\" || mkdir -p \"$$target\" && cp -r \"$$source\" \"$$target\"
            }
        }
    }
    !isEmpty(copyCommand) {
        copyCommand = @echo Copying application data... && $$copyCommand
        copydeploymentfolders.commands = $$copyCommand
        first.depends = $(first) copydeploymentfolders
        export(first.depends)
        export(copydeploymentfolders.commands)
        QMAKE_EXTRA_TARGETS += first copydeploymentfolders
    }
} else:ios {
    copyCommand =
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.source)
        source = $$replace(source, \\\\, /)
        target = $CODESIGNING_FOLDER_PATH/$$eval($${deploymentfolder}.target)
        target = $$replace(target, \\\\, /)
        sourcePathSegments = $$split(source, /)
        targetFullPath = $$target/$$last(sourcePathSegments)
        targetFullPath ~= s,/\\.?/,/,
        !isEqual(source,$$targetFullPath) {
            !isEmpty(copyCommand):copyCommand += &&
            copyCommand += mkdir -p \"$$target\"
            copyCommand += && cp -r \"$$source\" \"$$target\"
        }
    }
    !isEmpty(copyCommand) {
        copyCommand = echo Copying application data... && $$copyCommand
        !isEmpty(QMAKE_POST_LINK): QMAKE_POST_LINK += ";"
        QMAKE_POST_LINK += "$$copyCommand"
        export(QMAKE_POST_LINK)
    }
} else:unix {
    maemo5 {
        desktopfile.files = $${TARGET}.desktop
        desktopfile.path = /usr/share/applications/hildon
        icon.files = $${TARGET}64.png
        icon.path = /usr/share/icons/hicolor/64x64/apps
    } else:!isEmpty(MEEGO_VERSION_MAJOR) {
        desktopfile.files = $${TARGET}_harmattan.desktop
        desktopfile.path = /usr/share/applications
        icon.files = $${TARGET}80.png
        icon.path = /usr/share/icons/hicolor/80x80/apps
    } else { # Assumed to be a Desktop Unix
        copyCommand =
        for(deploymentfolder, DEPLOYMENTFOLDERS) {
            source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.source)
            source = $$replace(source, \\\\, /)
            macx {
                target = $$OUT_PWD/$${TARGET}.app/Contents/Resources/$$eval($${deploymentfolder}.target)
            } else {
                target = $$OUT_PWD/$$eval($${deploymentfolder}.target)
            }
            target = $$replace(target, \\\\, /)
            sourcePathSegments = $$split(source, /)
            targetFullPath = $$target/$$last(sourcePathSegments)
            targetFullPath ~= s,/\\.?/,/,
            !isEqual(source,$$targetFullPath) {
                !isEmpty(copyCommand):copyCommand += &&
                copyCommand += $(MKDIR) \"$$target\"
                copyCommand += && $(COPY_DIR) \"$$source\" \"$$target\"
            }
        }
        !isEmpty(copyCommand) {
            copyCommand = @echo Copying application data... && $$copyCommand
            copydeploymentfolders.commands = $$copyCommand
            first.depends = $(first) copydeploymentfolders
            export(first.depends)
            export(copydeploymentfolders.commands)
            QMAKE_EXTRA_TARGETS += first copydeploymentfolders
        }
    }
    !isEmpty(target.path) {
        installPrefix = $${target.path}
    } else {
        installPrefix = /opt/$${TARGET}
    }
    for(deploymentfolder, DEPLOYMENTFOLDERS) {
        item = item$${deploymentfolder}
        itemfiles = $${item}.files
        $$itemfiles = $$eval($${deploymentfolder}.source)
        itempath = $${item}.path
        $$itempath = $${installPrefix}/$$eval($${deploymentfolder}.target)
        export($$itemfiles)
        export($$itempath)
        INSTALLS += $$item
    }

    !isEmpty(desktopfile.path) {
        export(icon.files)
        export(icon.path)
        export(desktopfile.files)
        export(desktopfile.path)
        INSTALLS += icon desktopfile
    }

    isEmpty(target.path) {
        target.path = $${installPrefix}/bin
        export(target.path)
    }
    INSTALLS += target
}

export (ICON)
export (INSTALLS)
export (DEPLOYMENT)
export (LIBS)
export (QMAKE_EXTRA_TARGETS)
}

qtcAddDeployment()

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

QMAKE_INFO_PLIST = ios/Info.plist


