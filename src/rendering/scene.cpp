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

#include "scene.h"

Scene::Scene(Engine *engine)
{
    qDebug() << "Constructing Scene...";
    m_engine = engine;
    isSceneGLPrepared = false;
    isCurrentlyPicking = false;
    pick_x = 0;
    pick_y = 0;
    camera_x = 0.0f;
    camera_y = 0.0f;
    camera_z = 0.0f;
    bgR = 0.3f;
    bgG = 0.2f;
    bgB = 0.1f;
    bgA = 1.0f;
    supportsVAO = false;
    curTexID = -1;
}

void Scene::prepareScene()
{
    initializeOpenGLFunctions();

    QString versionString1(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    qDebug() << "OpenGL Driver Version String:" << versionString1;
    qDebug() << "Scene prepare - GL error check:" << glGetError();
    m_progSpr = loadShaders(Shader("SprShader.vsh"), Shader("SprShader.fsh"));
    m_progPick = loadShaders(Shader("PickShader.vsh"), Shader("PickShader.fsh"));
    qDebug() << "Scene prepare - Load shaders error check: " << glGetError();
    m_progSpr_Vertex = glGetAttribLocation(m_progSpr, "Vertex");
    m_progSpr_MVPMatrix = glGetUniformLocation(m_progSpr, "MVPMatrix");
    m_progSpr_UV = glGetAttribLocation(m_progSpr, "UV");
    m_progSpr_Texture = glGetUniformLocation(m_progSpr, "Texture");
    m_progSpr_AlphaColor = glGetUniformLocation(m_progSpr, "AlphaColor");
    m_progSpr_Brightness = glGetUniformLocation(m_progSpr, "Brightness");
    m_progSpr_Contrast = glGetUniformLocation(m_progSpr, "Contrast");
    m_progSpr_UVTransform = glGetUniformLocation(m_progSpr, "UVTransform");
    m_progPick_MVPMatrix = glGetUniformLocation(m_progPick, "MVPMatrix");
    m_progPick_Vertex = glGetAttribLocation(m_progPick, "Vertex");
    m_progPick_Color = glGetUniformLocation(m_progPick, "PickColor");
    qDebug() << "Scene prepare - Get Atribute/Uniform error check: " << glGetError();

    // Identity
    cameraMatrix.setToIdentity();

#if  __ANDROID_API__
    if(supportsVAO) {
        this->androOES = new QOpenGLExtension_OES_vertex_array_object();
        this->androOES->initializeOpenGLFunctions();
    }
#endif

    // Kada se dodaju meshevi i texture u scenu, ako je ovo true
    // onda se odmah poslije dodavanj au niz i bindaju u GL
    isSceneGLPrepared = true;

    // Posto se texture i meshevi mogu dodati u scenu prije nego je ona
    // gl initialized (tj. prije nego što se dobije kontekst)
    // sada bindati sve texture i mesheve.
    for(int n=0; n < m_engine->arrTextures.size(); n++) {
        if(!m_engine->arrTextures[n]->isGLBound) bindTexture(m_engine->arrTextures[n]);
    }

    for(int n=0; n < m_engine->arrMeshes.size(); n++) {
        if(!m_engine->arrMeshes[n]->isGLBound) bindMesh(m_engine->arrMeshes[n]);
    }
}

void Scene::setProjection()
{
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(orthoLeft, orthoRight,
                             orthoBottom, orthoTop,
                             orthoNear, orthoFar);
    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
}

void Scene::bindMesh(Mesh *mesh)
{
    // Ovo mora ispred VAO jer se samo jednom pune podacima (prije je bilo dole)
    // Napraviti i buffer ibo-a ovo treba kod iscrtavanja jer je array interlaved
    glGenBuffers(1, &mesh->IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexDataSize, mesh->indexData, GL_STATIC_DRAW);

    // uv, normale i vertex podaci su svi u jednom areju te ge kreirati u
    // openglu i upisati podatke
    glGenBuffers(1, &mesh->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh->vertexDataSize, mesh->vertexData, GL_STATIC_DRAW);


    /////////// VAOi za color pick render
#if OPENGLES_IOS
    glGenVertexArraysOES(1, &mesh->VAOPick);
    glBindVertexArrayOES(mesh->VAOPick);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glGenVertexArraysOES(1, &mesh->VAOPick);
        androOES->glBindVertexArrayOES(mesh->VAOPick);
    }
#elif OPENGL32
    glGenVertexArrays(1, &mesh->VAOPick);
    glBindVertexArray(mesh->VAOPick);
#endif

#if OPENGLES_IOS || OPENGLES_ANDRO || OPENGL32
    if(supportsVAO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glEnableVertexAttribArray(m_progPick_Vertex);
        glVertexAttribPointer(m_progPick_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
        glEnableVertexAttribArray(0);
    }
#endif

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(0);
    }
#elif OPENGL32
    glBindVertexArray(0);
#endif

    /////////// VAOi za sprite shader
#if OPENGLES_IOS
    glGenVertexArraysOES(1, &mesh->VAOSpr);
    glBindVertexArrayOES(mesh->VAOSpr);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glGenVertexArraysOES(1, &mesh->VAOSpr);
        androOES->glBindVertexArrayOES(mesh->VAOSpr);
    }
#elif OPENGL32
    glGenVertexArrays(1, &mesh->VAOSpr);
    glBindVertexArray(mesh->VAOSpr);
#endif

#if OPENGLES_IOS || OPENGLES_ANDRO || OPENGL32
    if(supportsVAO) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glEnableVertexAttribArray(m_progSpr_UV);
        glVertexAttribPointer(m_progSpr_UV, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(m_progSpr_Vertex);
        glVertexAttribPointer(m_progSpr_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
        glEnableVertexAttribArray(0);
    }
#endif

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(0);
    }
#elif OPENGL32
    glBindVertexArray(0);
#endif

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Scene::bindTexture(Texture *texture)
{
    glGenTextures(1, &texture->ID);
    glBindTexture(GL_TEXTURE_2D, texture->ID);

    QImage t;
    t = QGLWidget::convertToGLFormat( *texture->GetTextureData() );

    //GL_NEAREST_MIPMAP_LINEAR GL_NEAREST_MIPMAP_NEAREST GL_NEAREST

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
    glBindTexture(GL_TEXTURE_2D, 0);

    //TODO ako platforma podrzava mipmapiranje
    //Automatsko generisanje mipmap opngl 3+ i opengl es 2.0+
    //glBindTexture(GL_TEXTURE_2D, texture->ID);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0);
}

void Scene::renderModel(Model *model)
{
    QMatrix4x4 MVP = projectionMatrix * cameraMatrix * model->transformMatrix;

    glUniformMatrix4fv(m_progSpr_MVPMatrix, 1, 0, MVP.data());
    glUniform1f(m_progSpr_AlphaColor, model->alpha);
    glUniform1f(m_progSpr_Brightness, model->brightness);
    glUniform1f(m_progSpr_Contrast, model->contrast);
    glUniform1i(m_progSpr_Texture, 0);
    glUniformMatrix3fv(m_progSpr_UVTransform, 1, 0, model->UVTransform.data());

#if OPENGLES_IOS
    glBindVertexArrayOES(model->mesh->VAOSpr);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(model->mesh->VAOSpr);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
        glVertexAttribPointer(m_progSpr_UV, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(m_progSpr_UV);
        glVertexAttribPointer(m_progSpr_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
        glEnableVertexAttribArray(m_progSpr_Vertex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mesh->IBO);
    }
#elif OPENGL32
    glBindVertexArray(model->mesh->VAOSpr);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glVertexAttribPointer(m_progSpr_UV, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(m_progSpr_UV);
    glVertexAttribPointer(m_progSpr_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
    glEnableVertexAttribArray(m_progSpr_Vertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mesh->IBO);
#endif

    glDrawElements(GL_TRIANGLES,model->mesh->triangleCount*3,GL_UNSIGNED_INT,0);

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(0);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
#elif OPENGL32
    glBindVertexArray(0);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

void Scene::renderWireframe(const Transform &trans, const QColor &clr)
{
    //Transform transform;
    //transform.setPosition(obj->getPosition());
    //transform.setRotation(obj->getRotation());
    //transform.setSize(QVector3D(obj->getWidth(), obj->getHeight(), 1.0f));
    QMatrix4x4 MVP = projectionMatrix * cameraMatrix * trans.transformMatrix;
    glUniformMatrix4fv(m_progPick_MVPMatrix, 1, 0, MVP.data());
    glUniform4f(m_progPick_Color, clr.red(), clr.green(), clr.blue(), 1.0f);
    Mesh *mesh = m_engine->getMesh("DEFAULT_SPRITE_MESH");

#if OPENGLES_IOS
    glBindVertexArrayOES(mesh->VAOPick);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(mesh->VAOPick);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glVertexAttribPointer(m_progPick_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
        glEnableVertexAttribArray(m_progPick_Vertex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    }
#elif OPENGL32
    glBindVertexArray(model->mesh->VAOPick);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glVertexAttribPointer(m_progPick_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
    glEnableVertexAttribArray(m_progPick_Vertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
#endif
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES,mesh->triangleCount*3,GL_UNSIGNED_INT,0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(0);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
#elif OPENGL32
    glBindVertexArray(0);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif

}

void Scene::renderPick(Model *model)
{
    QMatrix4x4 MVP = projectionMatrix * cameraMatrix * model->transformMatrix;
    glUniformMatrix4fv(m_progPick_MVPMatrix, 1, 0, MVP.data());

    int r = (model->model_id & 0x000000FF) >>  0;
    int g = (model->model_id & 0x0000FF00) >>  8;
    int b = (model->model_id & 0x00FF0000) >> 16;

    glUniform4f(m_progPick_Color, r/255.0f, g/255.0f, b/255.0f, 1.0f);

#if OPENGLES_IOS
    glBindVertexArrayOES(model->mesh->VAOPick);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(model->mesh->VAOPick);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
        glVertexAttribPointer(m_progPick_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
        glEnableVertexAttribArray(m_progPick_Vertex);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mesh->IBO);
    }
#elif OPENGL32
    glBindVertexArray(model->mesh->VAOPick);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glVertexAttribPointer(m_progPick_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
    glEnableVertexAttribArray(m_progPick_Vertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mesh->IBO);
#endif

    glDrawElements(GL_TRIANGLES,model->mesh->triangleCount*3,GL_UNSIGNED_INT,0);

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    if(supportsVAO) {
        androOES->glBindVertexArrayOES(0);
    }
    else {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
#elif OPENGL32
    glBindVertexArray(0);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

int Scene::renderScenePick()
{
    for(int m=0; m < m_engine->arrModels.size(); m++) {
        if(m_engine->arrModels[m]->isVisible && m_engine->arrModels[m]->isSelectable) {
            renderPick(m_engine->arrModels[m]);
        }
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char data[4] = {0,0,0,0};
    glReadPixels(pick_x, pick_y, 1,1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
    int pickedID =
            data[0] +
            data[1] * 256 +
            data[2] * 256 * 256;

    return pickedID;
}

void Scene::pickPos(int x, int y)
{
    isCurrentlyPicking = true;
    pick_x = x;
    pick_y = y;
}

void Scene::renderScene()
{
    timer.start();

    qSort(m_engine->arrModels.begin(), m_engine->arrModels.end(), sortModels); //Prvo ih sortirati po z

    //TODO Hints po platfromama ako uopste trebaju?
    //glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DITHER);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    if(isCurrentlyPicking) {
        glUseProgram(m_progPick);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int pickedModelID = renderScenePick();
        emit objectPicked(pickedModelID);
        isCurrentlyPicking = false;
    }

    glClearColor(bgR, bgG, bgB, bgA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_progSpr);
    for(int m=0; m < m_engine->arrModels.size(); m++) {
        if(m_engine->arrModels[m]->isVisible) {
            if(curTexID == -1 || curTexID != m_engine->arrModels[m]->texture->ID) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_engine->arrModels[m]->texture->ID);
            }
            renderModel(m_engine->arrModels[m]);
        }
    }

#if DBUG_RENDER_PHYSICS
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_progPick);
    for(int p=0; p < m_engine->arrPhysicsShapes.size(); p++) {

        if(m_engine->arrPhysicsShapes.at(p)->offset.x() != 0.0f || m_engine->arrPhysicsShapes.at(p)->offset.y() != 0.0f) {
            Transform tp;
            tp.setPosition(m_engine->arrPhysicsShapes.at(p)->getPosition());
            tp.setSize(QVector3D(m_engine->arrPhysicsShapes.at(p)->getWidth(), m_engine->arrPhysicsShapes.at(p)->getHeight(), 1.0f));

            Transform t;
            t.setPosition(m_engine->arrPhysicsShapes.at(p)->getPosition() + m_engine->arrPhysicsShapes.at(p)->offset);
            t.setSize(QVector3D(m_engine->arrPhysicsShapes.at(p)->getWidth(),
                                        m_engine->arrPhysicsShapes.at(p)->getHeight(), 1.0f));
            tp.addChild(&t);
            tp.setRotation(m_engine->arrPhysicsShapes.at(p)->getRotation());
            renderWireframe(t, QColor(0,255,0));
        }
        else {
            Transform transform;
            transform.setPosition(m_engine->arrPhysicsShapes.at(p)->getPosition() + m_engine->arrPhysicsShapes.at(p)->offset );
            transform.setRotation(m_engine->arrPhysicsShapes.at(p)->getRotation());
            transform.setSize(QVector3D(m_engine->arrPhysicsShapes.at(p)->getWidth(),
                                        m_engine->arrPhysicsShapes.at(p)->getHeight(), 1.0f));
            renderWireframe(transform, QColor(0,255,0));
        }
    }
#endif

    timer.end200();

// Ovako moze samo kod spriteova koji nemaju alpha
//    glUseProgram(m_progSpr);
//    for(int t=0; t < textureList.size(); t++)
//    {
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, textureList[t]->ID);
//        for(int m=0; m < modelList.size(); m++) {
//            if(modelList[m]->isVisible && modelList[m]->texture->ID == textureList[t]->ID) {
//                renderModel(modelList[m]);
//            }
//        }
//    }
}


GLuint Scene::loadShaders(const QString &vsh, const QString &fsh)
{
    GLuint vertShader;
    GLuint fragShader;
    GLuint program;

    // Kreirati program
    program = glCreateProgram();

    // Ako kompajliranje shadra zajebe onda je je program koji se vraća iz
    // ove funkcije 0 to ionako ne smije biti jer id programa su non-zero
    if(!compileShader(&vertShader, GL_VERTEX_SHADER, vsh)) {
        return 0;
    }
    
    if(!compileShader(&fragShader, GL_FRAGMENT_SHADER, fsh)) {
        return 0;
    }
    
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    
    if (!linkProgram(program)) {
        qDebug() << "Shader link Error - program id:" << program;
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (program) {
            glDeleteProgram(program);
            program = 0;
        }
        return 0;
    }
    
    // Osloboditi shadere
    if (vertShader) {
        glDetachShader(program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(program, fragShader);
        glDeleteShader(fragShader);
    }

    return program;
}

char *Scene::loadFile(const char *path)
{
	FILE *fd;
	long len;
    long r;
	char *str;
    
	if (!(fd = fopen(path, "rb"))) {
        cout << "Greška prilikom otvaranja fajla:" << path << endl;
		return NULL;
	}
    
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
    
	if (!(str = (char*)malloc(len * sizeof(char)))) {
        cout << "Greška prilikom malloca:" << path << endl;
		return NULL;
	}
    
	r = fread(str, sizeof(char), len, fd);
    fclose(fd);
    // Dodati NULL na kraju
	str[r - 1] = '\0';

	return str;
}

int Scene::compileShader(GLuint *shader, GLenum type, const QString &file)
{
    //GLint status;
    const char *source;

    //source = loadFile(file);

    QFile f(file);
    f.open(QFile::ReadOnly | QFile::Text);
    QByteArray ba;
    ba = f.readAll();

    //if (!source) {
    //    cout << "Greška prilikom učitavanja shadera:" << file << endl;
    //    return 0;
    //}
    
    *shader = glCreateShader(type);
    //glShaderSource(*shader, 1, (const char**)&source, NULL);
    source = ba.constData();

    glShaderSource(*shader, 1, (const char**)&source, NULL);
    glCompileShader(*shader);
    //free(source);

    GLint status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(*shader, infoLogLength, NULL, strInfoLog);

        qDebug() << "Compile failure in shader:" << strInfoLog << "LENGHT " << infoLogLength ;
        delete[] strInfoLog;
    }

//#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        qDebug() << "Greška prilikom kompajliranja shadera:" << log;
        free(log);
    }
//#endif

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        qDebug() << "GL_COMPILE_STATUS Error";
        return 0;
    }


    return 1;
}

int Scene::linkProgram(GLuint program)
{
    GLint status;
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            GLchar *infolog = (GLchar *)malloc(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infolog);
            qDebug() << "Greška prilkom linkanja infolog:" << maxLength << "-----" << infolog;
            free(infolog);
            return 0;
    }
    
//#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        qDebug() << "Link status:" << log;
        free(log);
    }
//#endif
    
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0) {
        qDebug() << "GL_LINK_STATUS - ERR prodram id:" << program;
        return 0;
    }
    
    // TODO: napraviti da se u debug modu poziva
    //validate_program( program);

    return 1;
}

int Scene::validate_program(GLuint program)
{
    // Ova funkcija  je samo za developmenta trenutno se ne koristi

    GLint logLength, status;

    glValidateProgram(program);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        fprintf(stderr, "Program validate log:\n%s", log);
        free(log);
    }

    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return 0;
    }

    return 1;
}

Scene::~Scene()
{
    for(int n=0; n < m_engine->arrTextures.size(); n++) {
        glDeleteTextures(1, &m_engine->arrTextures[n]->ID);
    }
    
    for(int m=0; m < m_engine->arrModels.size(); m++) {
        if(m_engine->arrModels[m]) {
        //TODO: napraviti da briše mesheve i spritove
            //glDeleteBuffers(1, &m_modelList[m]->IBO);
            //glDeleteBuffers(1, &m_modelList[m]->VBO);
        }
    }

    //glDeleteTextures(1, &m_shadowTexture);
    //glDeleteFramebuffers(1, &m_shadowFBO);
    
    //glDeleteProgram(m_progShd);
    glDeleteProgram(m_progSpr);
    //glDeleteProgram(m_progStd);
    //glDeleteProgram(m_progDepth);
    glDeleteProgram(m_progPick);
}

bool Scene::sortModels(Model *a, Model *b)
{
    return a->getPosition().z() < b->getPosition().z();
}


