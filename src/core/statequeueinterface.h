#ifndef STATEQUEUEINTERFACE_H
#define STATEQUEUEINTERFACE_H

class StateQueueInterface
{
public:
    virtual void onStateEntered(int state);
    virtual void onStateExited(int state);

};

#endif // STATEQUEUEINTERFACE_H
