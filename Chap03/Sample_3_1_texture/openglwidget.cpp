#include "openglwidget.h"

OpenGlWidget::OpenGlWidget(QWidget *parent) : QOpenGLWidget(parent),QOpenGLFunctions_4_5_Core()
{

}


unsigned int VBO,VAO,EBO;
float vertices[] = {
    0.5f,0.5f,0.0f,    1.0f,0.0,0.0,   1.0f,1.0f,// top right
    0.5f,-0.5f,0.0f,   0.0,1.0f,0.0,   1.0f,0.0f,// top down

    -0.5f,-0.5f,0.0f,  0.0,0.0,1.0f,   0.0f,0.0f,// left down
    -0.5f,0.5f, 0.0f,  1.0f,1.0f,0.0,  0.0f,1.0f
};// left up

unsigned int indices[] = {
    0,1,2,
    0,2,3
};

OpenGlWidget::~OpenGlWidget()
{
    makeCurrent();
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&EBO);
    doneCurrent();
}

void OpenGlWidget::drawShape(OpenGlWidget::Shape Shape)
{
    makeCurrent();
    m_shape = Shape;
    update();
    doneCurrent();
}

void OpenGlWidget::setWireFrame(bool mode)
{
    makeCurrent();
    if(mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    update();
    doneCurrent();
}

void OpenGlWidget::initializeGL()
{


    // 初始化函数指针
    initializeOpenGLFunctions();


    // 生成一个VAO对象
    glGenVertexArrays(1,&VAO);
    // 生成一个VBO对象
    glGenBuffers(1,&VBO);

    // 绑定VAO和VBO
    // VAO对应唯一的子集
    glBindVertexArray(VAO);
    // VBO绑定到GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    // 向GL_ARRAY_BUFFER传送数据
    glBufferData(GL_ARRAY_BUFFER,sizeof (vertices),vertices,GL_STATIC_DRAW);

    // 告知显卡如何解析，把VBO的数据接到VAO的第0个插槽
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)0);
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)(sizeof(float)*3));
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof (float),(void*)(sizeof(float)*6));
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(2);



    glBindBuffer(GL_ARRAY_BUFFER,0);

    bool success;
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shapes.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shapes.frag");

    success = shaderProgram.link();
    if(!success)
    {
        qDebug() << "ERR:" << shaderProgram.log();
    }


    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof (indices),indices,GL_STATIC_DRAW);

    // opengl y轴与图片是反的
    texture = new QOpenGLTexture(QImage(":texture/texture/1.jpg").mirrored());
    texture1 = new QOpenGLTexture(QImage(":texture/texture/2.jpg").mirrored());

    shaderProgram.bind();
    shaderProgram.setUniformValue("texture0",0);
    shaderProgram.setUniformValue("texture1",1);
    // 释放助理
    glBindVertexArray(0);

    // 线框模式
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);



}

void OpenGlWidget::resizeGL(int w, int h)
{

}

void OpenGlWidget::paintGL()
{
    // set state
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    // use state
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.bind();
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES,0,6);

    switch (m_shape) {
        case Shape::None:
        {
            break;
        }
        case Shape::Rect:
        {
            texture->bind(0);
            texture1->bind(1);
            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
            break;
        }
        case Shape::Circle:
        {
            break;
        }
        case Shape::Triangle:
        {
            break;
        }
    }
    // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,&indices);
}
