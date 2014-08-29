#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>

class GameObject : public QObject
{
    Q_OBJECT
public:
    explicit GameObject(QObject *parent = 0);

    int type;
    int ID;
    QString name;

    virtual void update(float dt);
    virtual void keyPress(int key);
    virtual void keyRelease(int key);
};

#endif // GAMEOBJECT_H
