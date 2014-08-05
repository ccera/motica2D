
QT += gui opengl openglextensions

INCLUDEPATH +=  \
    src/engine \

HEADERS += \
    src/engine/texture.h \
    src/engine/sprite.h \
    src/engine/game.h \
    src/engine/label.h \
    src/engine/utils.h \
    src/engine/mesh.h \
    src/engine/model.h \
    src/engine/scene.h \
    src/engine/transform.h \
    src/engine/timer.h \
    mygame.h

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += \
    main.cpp \
    src/engine/texture.cpp \
    src/engine/sprite.cpp \
    src/engine/game.cpp \
    src/engine/label.cpp \
    src/engine/utils.cpp \
    src/engine/mesh.cpp \
    src/engine/model.cpp \
    src/engine/scene.cpp \
    src/engine/transform.cpp \
    src/engine/timer.cpp \
    mygame.cpp

# Ovdje dodati sve što se treba koirati uz app
resources_dir.source = resources
resources_dir.target = Resursi

shaders_150_dir.source = src/engine/Shaders_150
shaders_150_dir.target = Shaders

shaders_ES_dir.source = src/engine/Shaders_ES
shaders_ES_dir.target = Shaders

shaders_120_dir.source = src/engine/Shaders_120
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

