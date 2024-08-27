#ifndef LIGHT_H
#define LIGHT_H
#include <QVector3D>

class Light
{
public:
    Light();
    Light(QVector3D lightColor,QVector3D postion);
    QVector3D m_lightColor;
    QVector3D m_postion;

};

#endif // LIGHT_H
