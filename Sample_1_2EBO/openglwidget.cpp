#include "openglwidget.h"

OpenGlWidget::OpenGlWidget(QWidget *parent) : QOpenGLWidget(parent),QOpenGLFunctions_4_5_Core()
{

}
unsigned int VBO,VAO,EBO;
float vertices[] = {
    0.5f,0.5f,0.0f,  // top right
    0.5f,-0.5f,0.0f, // top down

    -0.5f,-0.5f,0.0f,// left down
    -0.5f,0.5f,0.0f};// left up

unsigned int indices[] = {
    0,1,2,
    0,2,3
};

const char *vertexShaderSource = "#version 450 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{ gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);}\0";

const char *fragmentShaderSource = "#version 450 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{ FragColor = vec4(1.0f,0.5f,0.2f,1.0);}\0";

unsigned int shaderProgram;

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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof (float),(void*)0);
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);


    // 编译着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    // 连接
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof (indices),indices,GL_STATIC_DRAW);

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

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES,0,6);

    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    // glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,&indices);
}
