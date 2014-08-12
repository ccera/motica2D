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

#include "game.h"

Game::Game(QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_animating(false)
    , m_context(0)
    , m_device(0)
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
    //format.setSwapInterval(1);
    this->setFormat(format);
#elif OPENGL32
    QSurfaceFormat format;
    format.setVersion(3,2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    format.setDepthBufferSize(32);
    format.setStencilBufferSize(8);
    this->setFormat(format);
#endif

    isPressed = false;
    scene = NULL;
    isGLInitialized = false;

    qApp->installEventFilter(this);

    scene = new Scene();
}

Game::~Game()
{
    delete m_device;
    delete scene;
}

void Game::render(QPainter *painter)
{
    //Painter labela.
    float height = painter->device()->height();
    for(int n=0; n < arrLabela.size(); n++) {
        painter->save();
        QRect rct(0 - (arrLabela[n]->transform->size_x/2.0f),
                  0 - (arrLabela[n]->transform->size_y/2.0f),
                  arrLabela[n]->transform->size_x,
                  arrLabela[n]->transform->size_y);
        painter->translate(arrLabela[n]->transform->x, height - arrLabela[n]->transform->y);
        painter->rotate(arrLabela[n]->transform->rot_z);
        QFont font;
        font.setFamily(arrLabela[n]->fontName);
        font.setPointSize(arrLabela[n]->fontSize);
        painter->setFont(font);
        painter->setPen(arrLabela[n]->color);
        painter->fillRect(rct, arrLabela[n]->backgroundColor);
        painter->drawText(rct, Qt::AlignCenter, arrLabela[n]->text);
        painter->restore();
    }

}

void Game::initialize()
{
    qDebug() << "Application initializing...";
    scene->prepareScene();
    scene->setProjection();
    connect(scene, SIGNAL(objectPicked(int)), this, SLOT(objectPicked(int)));
    isGLInitialized = true;
}

void Game::render()
{
    if (!m_device) {
        m_device = new QOpenGLPaintDevice(this->size()); //mora biti size
    }

    if (!isGLInitialized) {
        initialize();
    }

    QPainter painter;
    //m_device->setPaintFlipped(true); // ovako su normalne GL
    painter.begin(m_device);

    painter.beginNativePainting();
    if(scene) {
        scene->renderScene();
    }
    painter.endNativePainting();

    render(&painter);

    painter.end();

    // Send update to everybody. Delta time is in ms
    for(int n=0; n < arrEvents.size(); n++) {
        arrEvents.at(n)->update((1.0f / (float)screen()->refreshRate()) * 1000.0f);
    }

    emit update((1.0f / (float)screen()->refreshRate()) * 1000.0f);
}

void Game::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool Game::event(QEvent *event)
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

void Game::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed()) {
        renderNow();
    }
}

void Game::renderNow()
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

    if (!isGLInitialized) {
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating) {
        renderLater();
    }
}

void Game::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating) {
        renderLater();
    }
}

void Game::objectPicked(int modelId)
{
    for(int n=0; n < arrSprites.size(); n++) {
        if(arrSprites[n]->model_id == modelId) {
           arrSprites[n]->onPicked();
        }
    }

    for(int n=0; n < arrLabela.size(); n++) {
        if(arrLabela[n]->model_id == modelId) {
           arrLabela[n]->onPicked();
        }
    }
}


void Game::touchEvent(QTouchEvent *ev)
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

void Game::mousePressEvent(QMouseEvent *ev)
{
    isPressed = true;
    //TODO pozvati sve koji su prijavljeni na event
}

void Game::mouseMoveEvent(QMouseEvent *ev)
{
    if(isPressed) {
        //TODO pozvati sve koji su prijavljeni na event
    }
}

void Game::mouseReleaseEvent(QMouseEvent *ev)
{
    isPressed = false;
    //TODO pozvati sve koji su prijavljeni na event
}

void Game::keyPressEvent(QKeyEvent *ev)
{
    //TODO pozvati sve koji su prijavljeni na event
}

void Game::keyReleaseEvent(QKeyEvent *ev)
{
    //TODO pozvati sve koji su prijavljeni na event
}

bool Game::eventFilter(QObject *watched, QEvent* e)
{
    if(e->type() == QEvent::InputMethod) {
        QInputMethodEvent *m = (QInputMethodEvent*) e;
        qDebug() << "QInputMethodEvent commitString:" << m->commitString()
                 << " replacementLength:" << m->replacementLength()
                 << " replacementLength:" << m->replacementStart();
    }
    return QWindow::eventFilter(watched, e);
}

void Game::setWindowSize(float width, float height)
{
    this->resize(width, height);
    this->scene->viewportWidth = width;
    this->scene->viewportHeight = height;
    this->scene->orthoLeft = 0.0f;
    this->scene->orthoRight = width;
    this->scene->orthoBottom = 0.0f;
    this->scene->orthoTop = height;
    this->scene->orthoNear = -100.0f;
    this->scene->orthoFar = 100.0f;
    if(this->isGLInitialized) {
        this->scene->setProjection();
    }
}

void Game::setCamera2D(float x, float y, float width, float height)
{
    this->scene->orthoLeft = x;
    this->scene->orthoRight = width;
    this->scene->orthoBottom = y;
    this->scene->orthoTop = height;
    this->scene->orthoNear = -100.0f;
    this->scene->orthoFar = 100.0f;
    if(this->isGLInitialized) {
        this->scene->setProjection();
    }
}

void Game::setBackgroundColor(float r, float g, float b, float a)
{
    this->scene->bgR = r;
    this->scene->bgG = g;
    this->scene->bgB = b;
    this->scene->bgA = a;
}

void Game::showWindow()
{
    // TODO kasnije nacin prikazivanja razlicit po platformama ili da se
    // proslijedi kao parametar
    this->show();
}

void Game::addTexture(Texture *texture)
{
    this->scene->addTexture(texture);
}

void Game::addSprite(Sprite *sprite)
{
    this->scene->addModel(sprite);
    this->arrSprites.push_back(sprite);
}

void Game::addLabel(Label *label)
{
    this->scene->addModel(label);
    this->arrLabela.push_back(label);
}

void Game::connectToEvents(GameEvent *e)
{
    this->arrEvents.push_back(e);
}

