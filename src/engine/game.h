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

#ifndef GAME_H
#define GAME_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtCore/QCoreApplication>
#include <QInputMethod>
#include "texture.h"
#include "sprite.h"
#include "label.h"
#include "timer.h"
#include "scene.h"
#include "gameevent.h"

class Game : public QWindow
{
    Q_OBJECT
public:
    explicit Game(QWindow *parent = 0);
    ~Game();

    virtual void render(QPainter *painter);
    virtual void render();
    virtual void initialize();
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);
    virtual void touchEvent(QTouchEvent * ev);
    virtual bool eventFilter(QObject *, QEvent *);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseMoveEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void resizeEvent(QResizeEvent * ev);

    void setAnimating(bool animating);
    void setWindowSize(float width, float height);
    void setBackgroundColor(float r, float g, float b, float a);
    void showWindow();
    void addTexture(Texture *texture);
    void addSprite(Sprite *sprite);
    void addLabel(Label *label);
    void connectToEvents(GameEvent *e);
    void setCamera2DSize(float w, float h);
    void setCamera2DPos(float x, float y);
    void setViewport2DType(ViewportType type);

    Scene *scene;
    bool isPressed;
    bool isGLInitialized;
    QVector<Label*> arrLabela;
    QVector<Sprite*> arrSprites;
    QVector<GameEvent*> arrEvents;

signals:
    void update(float dt);

public slots:
    void renderLater();
    void renderNow();
    void objectPicked(int modelId);

protected:
    bool event(QEvent *event);
    void exposeEvent(QExposeEvent *event);

private:
    void calculateCamera();
    float m_camera_x;
    float m_camera_y;
    float m_camera_width;
    float m_camera_height;
    float m_camera_near;
    float m_camera_far;
    ViewportType m_viewport_type;

    bool m_update_pending;
    bool m_animating;
    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};

#endif // GAME_H

