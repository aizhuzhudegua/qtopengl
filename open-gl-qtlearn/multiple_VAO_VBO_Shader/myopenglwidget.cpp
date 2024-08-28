#include "myopenglwidget.h"
#include <QDebug>

GLuint MyOpenGLWidget::m_EBO = 0;

float firstTriangle[] = {
    -0.9f, -0.5f, 0.0f, // left
    -0.0f, -0.5f, 0.0f, // right
    -0.45f, 0.5f, 0.0f, // top
};

float secondTriangle[] = {
    0.0f, -0.5f, 0.0f, // left
    0.9f, -0.5f, 0.0f, // right
    0.45f, 0.5f, 0.0f // top
};


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    m_shape(None)
{

}

MyOpenGLWidget::~MyOpenGLWidget()
{
    makeCurrent();
    // 解除EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // 解除VBO
    glDeleteBuffers(2,m_VBO);
    // 解除VAO
    glDeleteVertexArrays(2,m_VAO);
    // 解除shader program
    m_shaderProgram.release();
    m_shaderProgram.removeAllShaders();
    m_shaderProgram1.release();
    m_shaderProgram1.removeAllShaders();
    doneCurrent();
}





void MyOpenGLWidget::initializeGL()
{
    /*
     * 初始化OpenGL的函数接口
     * 这个接口不能写在构造函数中，否则会程序运行出错，因为initializeOpenGLFunctions()中会需要窗口句柄，而窗口句柄在构造函数完成之后才会被创建
    */
    initializeOpenGLFunctions();
    /*
    * GL_MAX_VERTEX_ATTRIBS 对应的值至少为16个属性值
    * 返回一个值，即顶点着色器可访问的 4 分量通用顶点属性的最大数量。该值必须至少为16
    */
    GLint vertexAttributeCount;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributeCount);
    qDebug() << "GL_MAX_VERTEX_ATTRIBS count = " << vertexAttributeCount;

    // 创建VAO
    glGenVertexArrays(2, m_VAO);
    // 创建VBO
    glGenBuffers(2, m_VBO);

    // 绑定VAO和VBO对象
    glBindVertexArray(m_VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);

    // 为缓冲对象创建一个新的数据存储
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);

    /*
    * 定义一个通用顶点属性数据数组：告诉显卡如何解析缓冲里的属性值
    *   index:指定要修改的通用顶点属性的索引值
    *   size:指定通用顶点属性的分量，必须为1、2、3或4，初始值为4
    *   type:指定数组中每个元素得到类型，初始值为GL_FLOAT
    *   normalized:指定在访问定点数据值时是否应将其归一化 (GL_TRUE) 或直接转换为定点值 (GL_FALSE)
    *   stride:指定数组中每个通用顶点属性的步长
    *   pointer:指定通用顶点属性的属性的起始位置
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (void*)0);

    // 开启VAO管理的第一个属性值  VAO[0] 的第一个属性
    glEnableVertexAttribArray(0);

    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // 绑定VAO和VBO对象
    glBindVertexArray(m_VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);

    // 为缓冲对象创建一个新的数据存储
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);

    /*
    * 定义一个通用顶点属性数据数组：告诉显卡如何解析缓冲里的属性值
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (void*)0);

    // 开启VAO管理的第一个属性值  VAO[1] 的第一个属性
    glEnableVertexAttribArray(0);
    // 解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex_shader.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment_shader.frag");
    m_shaderProgram.link();

    m_shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex1_shader.vert");
    m_shaderProgram1.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment1_shader.frag");
    m_shaderProgram1.link();

}

void MyOpenGLWidget::paintGL()
{
    // 设置Clear的属性
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    // 使用
    glClear(GL_COLOR_BUFFER_BIT);

    
    m_shaderProgram1.bind();
    // 绘制图形
    //
    switch (m_shape)
    {
    case MyOpenGLWidget::None:
        break;
    case MyOpenGLWidget::Rect:
        break;
    case MyOpenGLWidget::Circle:
        break;
    case MyOpenGLWidget::Triangle:
    {
        // 绑定shader
        m_shaderProgram.bind();
        glBindVertexArray(m_VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        m_shaderProgram.release();
        
        m_shaderProgram1.bind();
        glBindVertexArray(m_VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        m_shaderProgram1.release();
        break;
    }
    case MyOpenGLWidget::Wireframe:
    {
        m_shaderProgram.bind();
        glBindVertexArray(m_VAO[0]);
        glDrawArrays(GL_LINE_LOOP, 0, 3);
        glBindVertexArray(0);
        m_shaderProgram.release();
        
        m_shaderProgram1.bind();
        glBindVertexArray(m_VAO[1]);
        glDrawArrays(GL_LINE_LOOP, 0, 3);
        glBindVertexArray(0);
        m_shaderProgram1.release();
        break;
    }
    default:
        break;
    }


}

void MyOpenGLWidget::drawShape(Shape shape)
{
    m_shape = shape;
    // 更新界面,重新绘制
    update();
}
