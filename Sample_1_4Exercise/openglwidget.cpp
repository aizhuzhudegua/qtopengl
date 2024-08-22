#include "openglwidget.h"

OpenGlWidget::OpenGlWidget(QWidget *parent) : QOpenGLWidget(parent),QOpenGLFunctions_4_5_Core()
{

}
unsigned int VBOs[2],VAOs[2];

float vertices[] = {
    // first triangle
    -0.9f,-0.5f,0.0f,
    0.0f, -0.5f,0.0f,
    -0.45f,0.5f,0.0f,
    // second triangle
    0.0f, -0.5f,0.0f,
    0.9f, -0.5f,0.0f,
    0.45f, 0.5f,0.0f,
};
float firstTriangle[] = {
    -0.9f,-0.5f,0.0f,
    0.0f, -0.5f,0.0f,
    -0.45f,0.5f,0.0f
};
float secondTriangle[] = {
    0.0f, -0.5f,0.0f,
    0.9f, -0.5f,0.0f,
    0.45f, 0.5f,0.0f,
};

const char *vertexShaderSource = "#version 450 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{ gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);}\0";

const char *fragmentShaderSource = "#version 450 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{ FragColor = vec4(1.0f,0.5f,0.2f,1.0);}\0";

const char *fragmentShaderSource2 = "#version 450 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{ FragColor = vec4(1.0f,1.0f,0f,1.0);}\0";

unsigned int shaderProgram;
unsigned int shaderProgram2;

void OpenGlWidget::initializeGL()
{


    // 初始化函数指针
    initializeOpenGLFunctions();


    // 生成2个VAO对象
    glGenVertexArrays(2,VAOs);
    // 生成2个VBO对象
    glGenBuffers(2,VBOs);

    // 绑定VAO和VBO
    // VAO对应唯一的子集
    glBindVertexArray(VAOs[0]);
    // VBO绑定到GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);


    // 在显存开辟缓冲区，数据记录在VBO内
    glBufferData(GL_ARRAY_BUFFER,sizeof (firstTriangle),firstTriangle,GL_STATIC_DRAW);

    // 告知显卡如何解析，把VBO的数据接到VAO的第0个插槽
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof (float),(void*)0);
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(0);

    // 绑定VAO和VBO
    // VAO对应唯一的子集
    glBindVertexArray(VAOs[1]);
    // VBO绑定到GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);


    // 在显存开辟缓冲区，数据记录在VBO内
    glBufferData(GL_ARRAY_BUFFER,sizeof (secondTriangle),secondTriangle,GL_STATIC_DRAW);

    // 告知显卡如何解析，把VBO的数据接到VAO的第0个插槽
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof (float),(void*)0);
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(0);

    // 编译着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2,1,&fragmentShaderSource2,NULL);
    glCompileShader(fragmentShader2);

    // 连接
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2,vertexShader);
    glAttachShader(shaderProgram2,fragmentShader2);
    glLinkProgram(shaderProgram2);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);
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
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES,0,3);

    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES,0,3);
}
