//
//  Copyright (c) 2014 Emir Cerić, Dejan Omasta. All rights reserved.
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
