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
