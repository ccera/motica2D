#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <QObject>
#include "scene.h"
#include "utils.h"
#include "scene.h"
#include "transform.h"
#include "model.h"
#include "texture.h"

class AnimatedSprite : public QObject, public Model
{
    Q_OBJECT

public:
    AnimatedSprite();
    AnimatedSprite(int numFrames, Texture *texture);

    void setTexture(Texture *p_texture);
    void setName(const QString &p_name);
    void setNumOfFrames(int num);
    void setFrameLength(float msec);
    void onPicked();
    void update(float dt);
    void setLoop(int from, int to);

signals:

public slots:


private:
    int counter;
    int m_frame_length;
    int m_loop_from;
    int m_loop_to;
    int direction;
};

#endif // ANIMATEDSPRITE_H
