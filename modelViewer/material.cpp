#include "material.h"

Material::Material()
{
    baseColor = QVector4D(1.0f,1.0f,1.0f,1.0f);
}

void Material::setBaseMap(QImage tex)
{
    BaseMap.loadTexture(tex);

}

void Material::setShader(QString vert, QString frag)
{
    m_shader.setVertexShader(vert);
    m_shader.setVertexShader(frag);
    m_shader.link();
}

void Material::use()
{
    m_shader.shaderProgram.bind();
    m_shader.shaderProgram.setUniformValue("baseMap",0);
    BaseMap.m_texture->bind(0);
}
