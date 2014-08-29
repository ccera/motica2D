#include "animatedsprite.h"

AnimatedSprite::AnimatedSprite() :
    QObject(), Model(NULL, texture, "")
{
    this->isAnimated = true;
    this->currentFrame = 0;
    this->numOfFrames = 1;
    //m_time = 0;
    m_frame_length = 3;
    m_loop_from = 0;
    m_loop_to = 0;
    counter = 0;
    direction = 1;
}

AnimatedSprite::AnimatedSprite(int numFrames, Texture *texture) :
    QObject(), Model(NULL, texture, "")
{
    this->isAnimated = true;
    this->currentFrame = 0;
    this->numOfFrames = numFrames;
    //m_time = 0;
    m_frame_length = 3;
    m_loop_from = 0;
    m_loop_to = 0;
    counter = 0;
    direction = 1;
}

void AnimatedSprite::setFrameLength(float msec)
{
    if(msec < 0) {
        m_frame_length = 0;
    }
    else {
        m_frame_length = msec;
    }
}

void AnimatedSprite::setNumOfFrames(int num)
{
    this->numOfFrames = num;
}

void AnimatedSprite::setTexture(Texture *p_texture)
{
    this->texture = p_texture;
}

void AnimatedSprite::setName(const QString &p_name)
{
    this->name = p_name;
}

void AnimatedSprite::onPicked()
{
    qDebug() << "AnimatedSprite::onPicked()" << this->model_id;
}

void AnimatedSprite::setLoop(int from, int to)
{
    if((to - from) > 0) {
        direction = 1;
    }
    else if ((to - from) < 0) {
        direction = -1;
    }
    else {
        direction = 0;
    }
    currentFrame = from;
    m_loop_from = from;
    m_loop_to = to;
    counter = 0;
    //m_time = 0;
}

void AnimatedSprite::update(float dt)
{
    if(direction == 0) {
        return;
    }

    if(counter  == (m_frame_length-1)) {
        currentFrame = currentFrame + direction;

        if( (direction == 1 && currentFrame > m_loop_to) || (direction == -1 && currentFrame < m_loop_to) ) {
                currentFrame = m_loop_from;
        }
    }

    counter = (counter + 1) % m_frame_length;
}
