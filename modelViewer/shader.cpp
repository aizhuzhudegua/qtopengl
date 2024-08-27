#include "shader.h"

Shader::Shader()
{

}

void Shader::setVertexShader(QString path)
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,path);
}

void Shader::setFragmentShader(QString path)
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,path);
}


bool Shader::link()
{
    bool success;
    success = shaderProgram.link();
    if(!success)
    {
        qDebug() << "ERR:" << shaderProgram.log();
    }
    return success;
}

void Shader::updateParameter()
{
    shaderProgram.bind();

}
