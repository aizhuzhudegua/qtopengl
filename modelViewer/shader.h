#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLShaderProgram>
#include "object.h"

class Shader:public Object
{  
public:
    Shader();
    void setVertexShader(QString path);
    void setFragmentShader(QString path);
    bool link();
    void updateParameter();

    QOpenGLShaderProgram shaderProgram;
};

#endif // SHADER_H
