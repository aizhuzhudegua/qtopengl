#include "light.h"

Light::Light()
{

}

Light::Light(QVector3D lightColor, QVector3D postion)
{
    m_lightColor = lightColor;
    m_postion = postion;
}
