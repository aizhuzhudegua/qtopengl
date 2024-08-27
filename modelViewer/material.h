#ifndef MATERIAL_H
#define MATERIAL_H
#include <QMap>
#include "object.h"
#include "shader.h"
#include "texture.h"


class Material: public Object
{
public:
    Material();
    void setBaseMap(QImage tex);
    void setShader(QString vert,QString frag);
    void use();

    Texture BaseMap;
    QVector4D baseColor;
    Shader m_shader;
private:

};

#endif // MATERIAL_H
