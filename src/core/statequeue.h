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

#ifndef STATEQUEUE_H
#define STATEQUEUE_H

#include <QObject>
#include <QQueue>
#include <QPair>
#include <QDebug>
#include "statequeueinterface.h"

class StateQueue : public QObject
{
    Q_OBJECT
public:
    explicit StateQueue(QObject *parent = 0);

    void setInterface(StateQueueInterface *interface);
    void enqueue(int state, int timer);
    void dequeue();
    void removeAll();
    void removeAllExceptCurrent();
    void update(float dt);
    int  currentState();

signals:

public slots:

private:
    QQueue<QPair<int, int> > m_stateQueue; //State, timer
    StateQueueInterface *m_interface;
    int timer;
};

#endif // STATEQUEUE_H
