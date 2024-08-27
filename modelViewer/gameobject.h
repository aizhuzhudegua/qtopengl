#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <QVector>
#include "component.h"

class GameObject
{
public:
    GameObject();
private:
    QVector<Component*> components;
};

#endif // GAMEOBJECT_H
