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

Scene::Scene()
{
    qDebug() << "Constructing Scene...";
    isSceneGLPrepared = false;
    curBoundVAO = 0;
    next_model_id = 1;
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

    initSpriteMesh();
}

void Scene::prepareScene()
{
    initializeOpenGLFunctions();

    qDebug() << "Scene prepare - GL error check:" << glGetError();

    m_progSpr = loadShaders(Shader("SprShader.vsh"), Shader("SprShader.fsh"));
    m_progPick = loadShaders(Shader("PickShader.vsh"), Shader("PickShader.fsh"));
    m_progDepth = loadShaders(Shader("DepthShader.vsh"), Shader("DepthShader.fsh"));

    qDebug() << "Scene prepare - Load shaders error check: " << glGetError();

    m_progSpr_Vertex = glGetAttribLocation(m_progSpr, "Vertex");
    m_progSpr_MVPMatrix = glGetUniformLocation(m_progSpr, "MVPMatrix");
    m_progSpr_UV = glGetAttribLocation(m_progSpr, "UV");
    m_progSpr_Texture = glGetUniformLocation(m_progSpr, "Texture");
    m_progSpr_AlphaColor = glGetUniformLocation(m_progSpr, "AlphaColor");
    m_progSpr_Brightness = glGetUniformLocation(m_progSpr, "Brightness");
    m_progSpr_Contrast = glGetUniformLocation(m_progSpr, "Contrast");
    
    m_progPick_MVPMatrix = glGetUniformLocation(m_progPick, "MVPMatrix");
    m_progPick_Vertex = glGetAttribLocation(m_progPick, "Vertex");
    m_progPick_Color = glGetUniformLocation(m_progPick, "PickColor");

    m_progDepth_Vertex = glGetAttribLocation(m_progDepth, "Vertex");
    m_progDepth_MVPMatrix = glGetUniformLocation(m_progDepth, "MVPMatrix");

    qDebug() << "Scene prepare - Get Atribute/Uniform error check: " << glGetError();

    // Identity
    cameraMatrix.setToIdentity();

#if  __ANDROID_API__
    this->androOES = new QOpenGLExtension_OES_vertex_array_object();
    this->androOES->initializeOpenGLFunctions();
#endif

    // Kada se dodaju meshevi i texture u scenu, ako je ovo true
    // onda se odmah poslije dodavanj au niz i bindaju u GL
    isSceneGLPrepared = true;

    // Posto se texture i meshevi mogu dodati u scenu prije nego je ona
    // gl initialized (tj. prije nego što se dobije kontekst)
    // sada bindati sve texture i mesheve.
    for(int n=0; n < textureList.size(); n++) {
        bindTexture(textureList[n]);
    }

    for(int n=0; n < meshList.size(); n++) {
        bindMesh(meshList[n]);
    }

}

void Scene::setProjection()
{
    projectionMatrix.setToIdentity();
    projectionMatrix.ortho(orthoLeft, orthoRight,
                             orthoBottom, orthoTop,
                             orthoNear, orthoFar);
    glViewport(0, 0, viewportWidth, viewportHeight);
}

void Scene::addMesh(Mesh *mesh)
{
    meshList.push_back(mesh);

    if(isSceneGLPrepared) {
        bindMesh(mesh);
    }
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


    /////////// VAOi za shadow map render
#if OPENGLES_IOS
    glGenVertexArraysOES(1, &mesh->VAOShdMap);
    glBindVertexArrayOES(mesh->VAOShdMap);
#elif OPENGLES_ANDRO
    androOES->glGenVertexArraysOES(1, &mesh->VAOShdMap);
    androOES->glBindVertexArrayOES(mesh->VAOShdMap);
#elif OPENGL32
    glGenVertexArrays(1, &mesh->VAOShdMap);
    glBindVertexArray(mesh->VAOShdMap);
#endif

#if OPENGLES_IOS || OPENGLES_ANDRO || OPENGL32
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glEnableVertexAttribArray(m_progDepth_Vertex);
    glVertexAttribPointer(m_progDepth_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
    glEnableVertexAttribArray(0);
#endif

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif  OPENGLES_ANDRO
    androOES->glBindVertexArrayOES(0);
#elif OPENGL32
    glBindVertexArray(0);
#endif


    /////////// VAOi za 3d color pick render
#if OPENGLES_IOS
    glGenVertexArraysOES(1, &mesh->VAOPick);
    glBindVertexArrayOES(mesh->VAOPick);
#elif OPENGLES_ANDRO
    androOES->glGenVertexArraysOES(1, &mesh->VAOPick);
    androOES->glBindVertexArrayOES(mesh->VAOPick);
#elif OPENGL32
    glGenVertexArrays(1, &mesh->VAOPick);
    glBindVertexArray(mesh->VAOPick);
#endif

#if OPENGLES_IOS || OPENGLES_ANDRO || OPENGL32
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glEnableVertexAttribArray(m_progPick_Vertex);
    glVertexAttribPointer(m_progPick_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
    glEnableVertexAttribArray(0);
#endif

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    androOES->glBindVertexArrayOES(0);
#elif OPENGL32
    glBindVertexArray(0);
#endif

    /////////// VAOi za sprite shader
#if OPENGLES_IOS
    glGenVertexArraysOES(1, &mesh->VAOSpr);
    glBindVertexArrayOES(mesh->VAOSpr);
#elif OPENGLES_ANDRO
    androOES->glGenVertexArraysOES(1, &mesh->VAOSpr);
    androOES->glBindVertexArrayOES(mesh->VAOSpr);
#elif OPENGL32
    glGenVertexArrays(1, &mesh->VAOSpr);
    glBindVertexArray(mesh->VAOSpr);
#endif

#if OPENGLES_IOS || OPENGLES_ANDRO || OPENGL32
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glEnableVertexAttribArray(m_progSpr_UV);
    glVertexAttribPointer(m_progSpr_UV, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(m_progSpr_Vertex);
    glVertexAttribPointer(m_progSpr_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
    glEnableVertexAttribArray(0);
#endif

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    androOES->glBindVertexArrayOES(0);
#elif OPENGL32
    glBindVertexArray(0);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


Mesh* Scene::getMesh(const QString &name)
{
    for(int n=0; n < meshList.size(); n++) {
        if(meshList[n]->name.compare(name) == 0) {
            return meshList[n];
        }
    }
    return 0;
}

void Scene::addTexture(Texture *texture)
{
    textureList.push_back(texture);

    if(isSceneGLPrepared) {
        bindTexture(texture);
    }
}

void Scene::bindTexture(Texture *texture)
{
    glGenTextures(1, &texture->ID);
    glBindTexture(GL_TEXTURE_2D, texture->ID);

    QImage t;
    QImage b;

    b.load(texture->filename);

//    QPainter painter;
//    painter.begin(&b);
//    painter.setPen(Qt::blue); // The font color comes from user select on a QColorDialog
//    painter.setFont(QFont("Arial", 30)); // The font size comes from user input
//    painter.setCompositionMode(QPainter::CompositionMode_Source);
//    painter.drawText(100, 100, "emir");  // Draw a number on the image
//    painter.end();


    t = QGLWidget::convertToGLFormat( b );

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glEnable(GL_TEXTURE_2D); // Ovo ne treba testirano na desk, androis još samo ios testirati

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
    glBindTexture(GL_TEXTURE_2D, 0);

    // Automatsko generisanje mipmap opngl 3+ i opengl es 2.0+
    //glBindTexture(GL_TEXTURE_2D, texture->ID);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, 0);
}

Texture* Scene::getTexture(const QString &name)
{
    for(int n=0; n < textureList.size(); n++) {
        if(textureList[n]->name.compare(name) == 0) {
            return textureList[n];
        }
    }
    return 0;
}

void Scene::initSpriteMesh()
{
    float vdata[] = {   0.0,0.0,  0.0,0.0,1.0,  -0.5,-0.5,0.0,
                        1.0,0.0,  0.0,0.0,1.0,   0.5,-0.5,0.0,
                        1.0,1.0,  0.0,0.0,1.0,   0.5, 0.5,0.0,
                        0.0,1.0,  0.0,0.0,1.0,  -0.5, 0.5,0.0
    };
    int idata[] = { 0,1,2,0,2,3, 0,2,1,0,3,2 };
    spriteMesh = new Mesh(vdata, idata, 8, 4, "DEFAULT_SPRITE_MESH");
    this->addMesh(spriteMesh);
}

void Scene::addModel(Model *model)
{
    model->mesh = this->getMesh("DEFAULT_SPRITE_MESH");
    model->model_id = next_model_id;
    next_model_id++;
    modelList.push_back(model);
}

void Scene::renderModel(Model *model)
{
    QMatrix4x4 MVP = projectionMatrix * cameraMatrix * model->transform->transformMatrix;

    glUniformMatrix4fv(m_progSpr_MVPMatrix, 1, 0, MVP.data());
    glUniform1f(m_progSpr_AlphaColor, model->alpha);
    glUniform1f(m_progSpr_Brightness, model->brightness);
    glUniform1f(m_progSpr_Contrast, model->contrast);
    glUniform1i(m_progSpr_Texture, 0);

#if OPENGLES_IOS
    glBindVertexArrayOES(model->mesh->VAOSpr);
#elif OPENGLES_ANDRO
    androOES->glBindVertexArrayOES(model->mesh->VAOSpr);
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
    androOES->glBindVertexArrayOES(0);
#elif OPENGL32
    glBindVertexArray(0);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

void Scene::renderOnlyDepth(Model *model)
{
    QMatrix4x4 MVP = projectionMatrix * cameraMatrix * model->transform->transformMatrix;
    glUniformMatrix4fv(m_progDepth_MVPMatrix, 1, 0, MVP.data());

#if OPENGLES_IOS
    glBindVertexArrayOES(model->mesh->VAOShdMap);
#elif OPENGLES_ANDRO
    androOES->glBindVertexArrayOES(model->mesh->VAOShdMap);
#elif OPENGL32
    glBindVertexArray(model->mesh->VAOShdMap);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, model->mesh->VBO);
    glVertexAttribPointer(m_progDepth_Vertex, 3, GL_FLOAT, GL_FALSE, 32, BUFFER_OFFSET(20));
    glEnableVertexAttribArray(m_progDepth_Vertex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->mesh->IBO);
#endif

    glDrawElements(GL_TRIANGLES,model->mesh->triangleCount*3,GL_UNSIGNED_INT,0);

#if OPENGLES_IOS
    glBindVertexArrayOES(0);
#elif OPENGLES_ANDRO
    androOES->glBindVertexArrayOES(0);
#elif OPENGL32
    glBindVertexArray(0);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

void Scene::renderPick(Model *model)
{
    QMatrix4x4 MVP = projectionMatrix * cameraMatrix * model->transform->transformMatrix;
    glUniformMatrix4fv(m_progPick_MVPMatrix, 1, 0, MVP.data());

    int r = (model->model_id & 0x000000FF) >>  0;
    int g = (model->model_id & 0x0000FF00) >>  8;
    int b = (model->model_id & 0x00FF0000) >> 16;

    glUniform4f(m_progPick_Color, r/255.0f, g/255.0f, b/255.0f, 1.0f);

#if OPENGLES_IOS
    glBindVertexArrayOES(model->mesh->VAOPick);
#elif OPENGLES_ANDRO
    androOES->glBindVertexArrayOES(model->mesh->VAOPick);
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
    androOES->glBindVertexArrayOES(0);
#elif OPENGL32
    glBindVertexArray(0);
#elif OPENGL21
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

int Scene::renderScenePick()
{
    for(int m=0; m < modelList.size(); m++) {
        if( (modelList[m]->isVisible && modelList[m]->isSelectable) ||
            (modelList[m]->isLabel && modelList[m]->isSelectable)
          )
        {
            renderPick(modelList[m]);
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
    //timer.start();

    for(int n=0; n < modelList.size(); n++) {
        modelList[n]->transform->isDirty = true;
        modelList[n]->transform->updateTransformMatrix();
    }
    qSort(modelList.begin(), modelList.end(), sortModels); //Prvo ih sortirati po z

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDisable(GL_DITHER);
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE); // Ukljuciti depth write (ovo je ionako deafultno)

    // Prvo se samo depth renda tako da svi iduci koraci uopšte ne upisuju depth, nego ga samo čitaju.
    // Kod ako ima više stageova i puno tekstura ovo drastično smanjuje vrijeme u fragment shaderu jer on
    // lako preskoči fragmente bazirano na depthu. Tako order vertexa ne utiče na brzinu rendanja
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, viewportWidth, viewportHeight);
    glUseProgram(m_progDepth);
    for(int m=0; m < modelList.size(); m++) {
        if(modelList[m]->isVisible) {
            renderOnlyDepth(modelList[m]);
        }
    }

    glDepthMask(GL_FALSE); // Isključiti depth write
    glDepthFunc(GL_LEQUAL); // Depth test mora biti ovo jer ako je defaultno LESS a pošto nda su fragmenti na istoj dubini
                            // mora biti LESS OR EQUAL

    if(isCurrentlyPicking) {
        glUseProgram(m_progPick);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor(0.0, 0.0, 0.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        int pickedModelID = renderScenePick();
        emit objectPicked(pickedModelID);
        isCurrentlyPicking = false;
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glClearColor(bgR, bgG, bgB, bgA);
    glClear(GL_COLOR_BUFFER_BIT); // Samo se čisti color, depth se ne smije dirati

    glUseProgram(m_progSpr);
    for(int t=0; t < textureList.size(); t++)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureList[t]->ID);
        for(int m=0; m < modelList.size(); m++) {
            if(modelList[m]->isVisible && modelList[m]->texture->ID == textureList[t]->ID) {
                renderModel(modelList[m]);
            }
        }
    }

    //timer.end200();

    // Vratiti sve na defaultne postavke jer QT treba da iscrta svoje
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_ZERO, GL_ZERO);
    glDisable(GL_BLEND);
    glEnable(GL_DITHER);
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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
    for(int n=0; n < textureList.size(); n++) {
        glDeleteTextures(1, &textureList[n]->ID);
    }
    
    for(int m=0; m < modelList.size(); m++) {
        if(modelList[m]) {
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
    glDeleteProgram(m_progDepth);
    glDeleteProgram(m_progPick);
}

bool Scene::sortModels(Model *a, Model *b)
{
    return a->transform->z < b->transform->z;
}


