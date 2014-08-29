#include "statequeue.h"

StateQueue::StateQueue(QObject *parent) :
    QObject(parent)
{
    m_interface = 0;
    timer = 0;
}

void StateQueue::setInterface(StateQueueInterface *interface)
{
    m_interface = interface;
}

void StateQueue::enqueue(int state, int timer)
{
    m_stateQueue.enqueue(QPair<int, int>(state,timer));
    timer = 0;
    if(m_interface && m_stateQueue.size() == 1) {
        m_interface->onStateEntered(state);
    }
}

void StateQueue::dequeue()
{
    if(m_interface) {
        m_interface->onStateExited(this->currentState());
    }
    m_stateQueue.dequeue();
    timer = 0;
}

void StateQueue::removeAll()
{
    if(m_interface) {
        m_interface->onStateExited(this->currentState());
    }
    while(m_stateQueue.size() > 0) {
        m_stateQueue.dequeue();
    }
    timer = 0;
}

void StateQueue::removeAllExceptCurrent()
{
    while(m_stateQueue.size() > 1) {
        m_stateQueue.removeLast();
    }
}

void StateQueue::update(float dt)
{
    timer++;

    if(m_stateQueue.head().second == timer) {
        this->dequeue();
        if(m_interface && m_stateQueue.size() > 0) {
            m_interface->onStateEntered(m_stateQueue.head().first);
        }
    }
}

int  StateQueue::currentState()
{
    return m_stateQueue.head().first;
}
