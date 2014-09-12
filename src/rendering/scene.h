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

#ifndef __Scene__
#define __Scene__

#include <QObject>
#include "utils.h"
#include "model.h"
#include "texture.h"
#include "timer.h"
#include <QImage>
#include <QOpenGLFunctions>
#include <QOpenGLExtensions>
#include <QReadWriteLock>
#include <QMatrix4x4>
#include <QQuaternion>
#include "physicsobject.h"

class Scene  : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Scene();
    ~Scene();

    QReadWriteLock  lock;

    void    prepareScene();
    void    setProjection();
    void    addTexture(Texture *texture);
    void    addModel(Model *model);
    void    addMesh(Mesh *mesh);
    void    addPhysicsObject(PhysicsObject *object);
    void    bindMesh(Mesh *mesh);
    void    bindTexture(Texture *texture);
    void    renderScene();
    void    pickPos(int x, int y);
    Texture*  getTexture(const QString &name);
    Mesh*     getMesh(const QString &name);
    
    // Shader loader
    // TODO: ocajni su popraviti nekad
    GLuint   loadShaders(const QString &vsh, const QString &fsh);
    char*    loadFile(const char *path);
    int      compileShader(GLuint *shader, GLenum type, const QString &file);
    int      linkProgram(GLuint program);
    int      validate_program(GLuint program);
    
    //Defaultni sprite mesh(kvadrat)
    Mesh *spriteMesh;

    QMatrix4x4 projectionMatrix;
    QMatrix4x4 cameraMatrix;

    // Varijable pozicije i rotacije kamere
    float camera_x;
    float camera_y;
    float camera_z;

    GLuint viewportWidth;
    GLuint viewportHeight;
    GLuint viewportX;
    GLuint viewportY;

    float orthoLeft;
    float orthoRight;
    float orthoTop;
    float orthoBottom;
    float orthoNear;
    float orthoFar;

    // Background color glClear
    float bgR;
    float bgG;
    float bgB;
    float bgA;

    // Liste textura i glavna render lista svih modela za ovu scenu
    QVector <Texture*> textureList;
    QVector <Mesh*> meshList;
    QVector <Model*> modelList;
    QVector <PhysicsObject*> physicsObjectList;

    bool supportsVAO;


signals:
    void objectPicked(int modelID);

public slots:

protected:
    void    renderModel(Model *model);
    void    renderPick(Model *model);
    int     renderScenePick();
    void    renderPhysics(PhysicsObject *obj);
    void    initSpriteMesh();
    static bool sortModels(Model *a, Model *b);

    bool isSceneGLPrepared;

#if  __ANDROID_API__
    QOpenGLExtension_OES_vertex_array_object *androOES;
#endif

    // ID GLSL programa
    GLuint m_progSpr; // Za rendanje spriteova nema lighta i shadinga normala itd
    GLuint m_progPick; // Za rendanje samo boja koristi se kod 3d pickanja objekata

    // Lokacije uniform varijabli sprite programa
    GLint m_progSpr_MVPMatrix;
    GLint m_progSpr_Vertex;
    GLint m_progSpr_Texture;
    GLint m_progSpr_UV;
    GLint m_progSpr_AlphaColor;
    GLint m_progSpr_Brightness;
    GLint m_progSpr_Contrast;
    GLint m_progSpr_UVTransform;
    
    // Lokacije uniform varijabli 3d picker programa
    GLint m_progPick_MVPMatrix;
    GLint m_progPick_Vertex;
    GLint m_progPick_Color;

    int next_model_id; // Id koji se dodjeljuje svakom modelu
    bool isCurrentlyPicking; // Da li se ovaj frame pickaju opbjekti
    int pick_x;
    int pick_y;
    
    Timer timer;

    GLuint curTexID;

private:

};

#endif
