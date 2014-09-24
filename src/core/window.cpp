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

#include "window.h"

Window::Window(Engine *engine)
    : QWindow()
    , m_update_pending(false)
    , m_animating(false)
    , m_context(0)
    , m_device(0)
    , m_engine(engine)
{
    setSurfaceType(QWindow::OpenGLSurface);

#if OPENGLES_IOS || OPENGLES_ANDRO
    //Za sad ništa ne raditi samo se namjesti
#elif OPENGL21
    QSurfaceFormat format;
    format.setVersion(2,1);
    format.setSamples(4);
    format.setDepthBufferSize(16); //32 ne radi na linux
    format.setStencilBufferSize(8);
    format.setSwapInterval(1);
    this->setFormat(format);
#elif OPENGL32
    QSurfaceFormat format;
    format.setVersion(3,2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setSamples(4);
    //format.setDepthBufferSize(32);
    //format.setStencilBufferSize(8);
    this->setFormat(format);
#endif

    engine->engine = this;
    isPressed = false;
    m_engine->scene = NULL;
    m_engine->isGLInitialized = false;

    qApp->installEventFilter(this);

    m_engine->scene = new Scene(m_engine);
    m_engine->physicsWorld = new PhysicsWorld();

    m_engine->camera_x = 0.0f;
    m_engine->camera_y = 0.0f;
    m_engine->camera_width = 0;
    m_engine->camera_height = 0;
    m_engine->camera_near = -100.0f;
    m_engine->camera_far = 100.0f;
    m_engine->viewport_type = VIEWPORT_STRECH_Y;

//
// Default screen settings for different platforms
//
#if _WIN64 || _WIN32 || (__linux && !__ANDROID_API__)
    this->resize(1024, 512);
#elif __APPLE__
   #include "TargetConditionals.h"
   #if TARGET_OS_IPHONE
        this->resize(this->screen()->size());
        this->resize(this->screen()->size());
   #elif TARGET_OS_MAC
        this->resize(1024, 512);
   #endif
#elif __ANDROID_API__
    this->resize(this->screen()->size());
#elif __linux
    this->resize(1024, 512);
#endif
////

}

Window::~Window()
{
    delete m_device;
    //delete scene;
}

void Window::initialize()
{
    qDebug() << "Application initializing...";

#if OPENGLES_ANDRO
    if(m_context->hasExtension("GL_OES_vertex_array_object")) {
        scene->supportsVAO = true;
        qDebug() << "Supports GL_OES_vertex_array_object";
    }
    else {
        scene->supportsVAO = false;
        qDebug() << "Does not support GL_OES_vertex_array_object";
    }
#elif OPENGLES_IOS || OPENGL32
    scene->supportsVAO = true;
#endif

    m_engine->scene->prepareScene();
    m_engine->scene->setProjection();
    connect(m_engine->scene, SIGNAL(objectPicked(int)), this, SLOT(objectPicked(int)));
    m_engine->isGLInitialized = true;
}

void Window::render()
{
    if (!m_device) {
        m_device = new QOpenGLPaintDevice();
    }

    if (!m_engine->isGLInitialized) {
        initialize();
    }

    // Update physics
    //qDebug() << (1.0f / (float)screen()->refreshRate()) << (1.0f/60.0f) << (1.0f/30.0f);
    m_engine->physicsWorld->updateWorld(1.0f/30.0f); //((1.0f / (float)screen()->refreshRate()) * 1000.0f);

    for(int n=0; n < m_engine->arrPhysicsObjects.size(); n++) {
        m_engine->arrPhysicsObjects.at(n)->update((1.0f / (float)screen()->refreshRate()) * 1000.0f);
    }

    // Send update to everybody. Delta time is in ms
    for(int n=0; n < m_engine->arrGameObjects.size(); n++) {
        m_engine->arrGameObjects.at(n)->update((1.0f / (float)screen()->refreshRate()) * 1000.0f);
    }

    for(int n=0; n < m_engine->arrAnimSprites.size(); n++) {
        m_engine->arrAnimSprites.at(n)->update((1.0f / (float)screen()->refreshRate()) * 1000.0f);
    }

    if(m_engine->scene) {
        this->calculateCamera();
        m_engine->scene->renderScene();
    }
}

void Window::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool Window::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void Window::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed()) {
        renderNow();
    }
}

void Window::renderNow()
{
    if (!isExposed()) {
        return;
    }

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
    }

    m_context->makeCurrent(this);

    if (!m_engine->isGLInitialized) {
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating) {
        renderLater();
    }
}

void Window::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating) {
        renderLater();
    }
}

void Window::objectPicked(int modelId)
{
    for(int n=0; n < m_engine->arrSprites.size(); n++) {
        if(m_engine->arrSprites[n]->model_id == modelId) {
           m_engine->arrSprites[n]->onPicked();
        }
    }
}


void Window::touchEvent(QTouchEvent *ev)
{
    if(ev->type() == QTouchEvent::TouchBegin)
    {
        isPressed = true;
        //TODO pozvati sve koji su prijavljeni na event
    }

    if(ev->type() == QTouchEvent::TouchUpdate)
    {
        if(isPressed) {
            //TODO pozvati sve koji su prijavljeni na event
        }
    }

    if(ev->type() == QTouchEvent::TouchEnd)
    {
        isPressed = false;
        //TODO pozvati sve koji su prijavljeni na event
    }

    if(ev->type() == QTouchEvent::TouchCancel)
    {
        //TODO Sta sa ovim ??
    }
}

void Window::mousePressEvent(QMouseEvent *ev)
{
    isPressed = true;
    //TODO pozvati sve koji su prijavljeni na event
}

void Window::mouseMoveEvent(QMouseEvent *ev)
{
    if(isPressed) {
        //TODO pozvati sve koji su prijavljeni na event
    }
}

void Window::mouseReleaseEvent(QMouseEvent *ev)
{
    isPressed = false;
    //TODO pozvati sve koji su prijavljeni na event
}

void Window::keyPressEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up) Keyboard::keyUP = true;
    if(ev->key() == Qt::Key_Down) Keyboard::keyDOWN = true;
    if(ev->key() == Qt::Key_Left) Keyboard::keyLEFT = true;
    if(ev->key() == Qt::Key_Right) Keyboard::keyRIGHT = true;

    if(!ev->isAutoRepeat()) {
        for(int n=0; n < m_engine->arrGameObjects.size(); n++) {
            m_engine->arrGameObjects.at(n)->keyPress(ev->key());
        }
    }
}

void Window::keyReleaseEvent(QKeyEvent *ev)
{
    if(ev->key() == Qt::Key_Up) Keyboard::keyUP = false;
    if(ev->key() == Qt::Key_Down) Keyboard::keyDOWN = false;
    if(ev->key() == Qt::Key_Left) Keyboard::keyLEFT = false;
    if(ev->key() == Qt::Key_Right) Keyboard::keyRIGHT = false;

    if(!ev->isAutoRepeat()) {
        for(int n=0; n < m_engine->arrGameObjects.size(); n++) {
            m_engine->arrGameObjects.at(n)->keyRelease(ev->key());
        }
    }
}

bool Window::eventFilter(QObject *watched, QEvent* e)
{
    if(e->type() == QEvent::InputMethod) {
        QInputMethodEvent *m = (QInputMethodEvent*) e;
        qDebug() << "QInputMethodEvent commitString:" << m->commitString()
                 << " replacementLength:" << m->replacementLength()
                 << " replacementLength:" << m->replacementStart();
    }
    return QWindow::eventFilter(watched, e);
}

void Window::resizeEvent(QResizeEvent *ev)
{
    this->calculateCamera();
}

void Window::setWindowSize(float width, float height)
{
    this->resize(width, height);
    this->calculateCamera();
}

void Window::setCamera2DSize(float w, float h)
{
    m_engine->camera_width = w;
    m_engine->camera_height = h;
    this->calculateCamera();
}

void Window::setCamera2DPos(float x, float y)
{
    m_engine->camera_x = x;
    m_engine->camera_y = y;
    this->calculateCamera();
}

void Window::setViewport2DType(ViewportType type)
{
    m_engine->viewport_type = type;
    this->calculateCamera();
}

void Window::calculateCamera()
{
    if(m_engine->viewport_type == VIEWPORT_PIXEL) {
        m_engine->scene->viewportX = 0;
        m_engine->scene->viewportY = 0;
        m_engine->scene->viewportWidth = m_engine->camera_width;
        m_engine->scene->viewportHeight = m_engine->camera_height;
        m_engine->scene->orthoLeft = m_engine->camera_x;
        m_engine->scene->orthoRight = m_engine->camera_width;
        m_engine->scene->orthoBottom = m_engine->camera_y;
        m_engine->scene->orthoTop =  m_engine->camera_height;
        m_engine->scene->orthoNear = m_engine->camera_near;
        m_engine->scene->orthoFar = m_engine->camera_far;
        if(m_engine->isGLInitialized) { m_engine->scene->setProjection(); }
    }

    if(m_engine->viewport_type == VIEWPORT_STRECH_X) {
        float screen_height = (float)this->size().height();
        float screen_width = (float)this->size().width();
        float e = screen_width / m_engine->camera_width;
        float d = (screen_height - (m_engine->camera_height * e)) / 2.0f;
        m_engine->scene->viewportX = 0;
        m_engine->scene->viewportY = 0 + d;
        m_engine->scene->viewportWidth = m_engine->camera_width * e;
        m_engine->scene->viewportHeight = m_engine->camera_height * e;
        m_engine->scene->orthoLeft = m_engine->camera_x;
        m_engine->scene->orthoRight = m_engine->camera_width;
        m_engine->scene->orthoBottom = m_engine->camera_y ;
        m_engine->scene->orthoTop =  m_engine->camera_height;
        m_engine->scene->orthoNear = m_engine->camera_near;
        m_engine->scene->orthoFar = m_engine->camera_far;
        if(m_engine->isGLInitialized) { m_engine->scene->setProjection(); }
    }

    if(m_engine->viewport_type == VIEWPORT_STRECH_Y) {
        float screen_height = (float)this->size().height();
        float screen_width = (float)this->size().width();
        float e = screen_height / m_engine->camera_height;
        float d = (screen_width - (m_engine->camera_width * e)) / 2.0f;
        m_engine->scene->viewportX = 0 + d;
        m_engine->scene->viewportY = 0;
        m_engine->scene->viewportWidth = m_engine->camera_width * e;
        m_engine->scene->viewportHeight = m_engine->camera_height * e;
        m_engine->scene->orthoLeft = m_engine->camera_x;
        m_engine->scene->orthoRight = m_engine->camera_width;
        m_engine->scene->orthoBottom = m_engine->camera_y ;
        m_engine->scene->orthoTop =  m_engine->camera_height;
        m_engine->scene->orthoNear = m_engine->camera_near;
        m_engine->scene->orthoFar = m_engine->camera_far;
        if(m_engine->isGLInitialized) { m_engine->scene->setProjection(); }
    }

    if(m_engine->viewport_type == VIEWPORT_STRECH_XY) {
        m_engine->scene->viewportX = 0;
        m_engine->scene->viewportY = 0;
        m_engine->scene->viewportWidth = this->size().width();
        m_engine->scene->viewportHeight = this->size().height();
        m_engine->scene->orthoLeft = m_engine->camera_x;
        m_engine->scene->orthoRight = m_engine->camera_width;
        m_engine->scene->orthoBottom = m_engine->camera_y;
        m_engine->scene->orthoTop =  m_engine->camera_height;
        m_engine->scene->orthoNear = m_engine->camera_near;
        m_engine->scene->orthoFar = m_engine->camera_far;
        if(m_engine->isGLInitialized) { m_engine->scene->setProjection(); }
    }
}

void Window::setBackgroundColor(float r, float g, float b, float a)
{
    m_engine->scene->bgR = r;
    m_engine->scene->bgG = g;
    m_engine->scene->bgB = b;
    m_engine->scene->bgA = a;
}

void Window::showWindow()
{
#if _WIN64 || _WIN32 || (__linux && !__ANDROID_API__)
   this->show();
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        this->showFullScreen();
    #elif TARGET_OS_MAC
         this->show();
    #endif
#elif __ANDROID_API__
    //this->showFullScreen();
    this->show();
#elif __linux
    this->show();
#endif
}
