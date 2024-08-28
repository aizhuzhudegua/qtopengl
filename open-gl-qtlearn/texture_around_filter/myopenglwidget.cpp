#include "myopenglwidget.h"
#include <QDebug>
#include <QTime>
#include <QImage>
#include <QColor>
#include <QOpenGLFunctions>
GLuint MyOpenGLWidget::m_EBO = 0;

// 环绕方式使用
float vertices[] = {
    // positions        // colors               // texture coords
    0.9f, 0.9f, 0.0f,   1.0f, 0.0f, 0.0f,       2.0f, 2.0f, // top right
    0.9f, -0.9f, 0.0f,  0.0f, 1.0f, 0.0f,       2.0f, -1.0f, // bottom right
    -0.9f, -0.9f, 0.0f, 0.0f, 0.0f, 1.0f,       -1.0f, -1.0f, // bottom left
    -0.9f, 0.9f, 0.0f,  1.0f, 1.0f, 0.0f,       -1.0f, 2.0f // top left
};

// 纹理过滤使用
float verticesFilter[] = {
    // positions        // colors               // texture coords
    0.9f, 0.9f, 0.0f,   1.0f, 0.0f, 0.0f,       1.0f, 1.0f, // top right
    0.9f, -0.9f, 0.0f,  0.0f, 1.0f, 0.0f,       1.0f, 0.0f, // bottom right
    -0.9f, -0.9f, 0.0f, 0.0f, 0.0f, 1.0f,       0.0f, 0.0f, // bottom left
    -0.9f, 0.9f, 0.0f,  1.0f, 1.0f, 0.0f,       0.0f, 1.0f // top left
};

GLuint index[] = {
    0,1,3,
    1,2,3
};


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    m_shape(None)
{

}

MyOpenGLWidget::~MyOpenGLWidget()
{
    if (!isValid())
        return;
    makeCurrent();
    // 解除EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // 解除VBO
    glDeleteBuffers(1,&m_VBO);
    // 解除VAO
    glDeleteVertexArrays(1,&m_VAO);
    // 解除shader program
    m_shaderProgram.release();
    m_shaderProgram.removeAllShaders();
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
    glGenVertexArrays(1,&m_VAO);
    // 创建VBO
    glGenBuffers(1,&m_VBO);

    // 绑定VAO和VBO对象
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,m_VBO);

    // 为缓冲对象创建一个新的数据存储
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices,GL_STATIC_DRAW);

    /*
    * 定义一个通用顶点属性数据数组：告诉显卡如何解析缓冲里的属性值
    *   index:指定要修改的通用顶点属性的索引值
    *   size:指定通用顶点属性的分量，必须为1、2、3或4，初始值为4
    *   type:指定数组中每个元素得到类型，初始值为GL_FLOAT
    *   normalized:指定在访问定点数据值时是否应将其归一化 (GL_TRUE) 或直接转换为定点值 (GL_FALSE)
    *   stride:指定数组中每个通用顶点属性的步长
    *   pointer:指定通用顶点属性的属性的起始位置
    */ 
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,8 * sizeof(GLfloat),(void*)0);
    // 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    // 开启VAO管理的第二个属性值
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    // 开启VAO管理的第三个属性值
    glEnableVertexAttribArray(2);
    
    
    // 创建EBO,必须在VAO解绑之前创建，因为EBO也会被VAO所记录，但是解绑VBO却不会对VAO中的记录产生影响
    glGenBuffers(1,&m_EBO);
    // 绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_EBO);
    // 给EBO分配空间，并存储数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(index),index,GL_STATIC_DRAW);

    // 解除EBO，若此处解绑EBO则VAO中就会不在记录EBO内容，需要在使用的时候重新绑定EBO，这里放在析构函数中解绑
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // 解除VBO和VAO的绑定
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //glBindVertexArray(0);



    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex_shader.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment_shader.frag");
    m_shaderProgram.link();
    
    // 设置纹理，并将y轴进行镜像
    m_texture = std::make_unique<QOpenGLTexture>(QImage(":/images/images/small.png").mirrored());
    

    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("texture", 0);
    m_shaderProgram.release();
    


}

void MyOpenGLWidget::paintGL()
{
    // 设置Clear的属性
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    // 使用
    glClear(GL_COLOR_BUFFER_BIT);

    // 绑定shader
    m_shaderProgram.bind();
    

    // 绘制图形
    //
    switch (m_shape)
    {
    case MyOpenGLWidget::None:
        break;
    case MyOpenGLWidget::Rect:
    {

        
        m_texture->bind();
        // 1. TS镜像重复
        m_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::MirroredRepeat);
        m_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::MirroredRepeat);
        // 2. 处理到边界，剩下的进行边界颜色填充
        //m_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToBorder);
        //m_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToBorder);
        //m_texture->setBorderColor(1.0f, 1.0f, 0.0f, 1.0f);
        // 3. 延伸到边界
        //m_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
        //m_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
        // 设置放大时纹理过滤
        m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
        // 设置缩小时纹理过滤
        //m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
        /*
        * 多级渐远纹理
        * 一个常见的错误是，将放大过滤的选项设置为多级渐远纹理过滤选项之一。
        *   这样没有任何效果，因为多级渐远纹理主要是使用在纹理被缩小的情况下的：纹理放大不会使用多级渐远纹理，
        *       为放大过滤设置多级渐远纹理的选项会产生一个GL_INVALID_ENUM错误代码
        */ 
        //m_texture->generateMipMaps();
        //m_texture->setMinificationFilter(QOpenGLTexture::NearestMipMapNearest);
        // 指定绘图的模式，这里设定为GL_LINE，此模式只绘制线框而不进行填充
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        /*
         * 根据索引绘制
         * 若没有创建EBO，最后一个参数可以直接传递索引的数组
        */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL/*&index*/);
        m_texture->release();
        break;
    }
    case MyOpenGLWidget::Circle:
        break;
    case MyOpenGLWidget::Triangle:
        break;
    case MyOpenGLWidget::RectWireframe:
    {
        glDrawArrays(GL_LINE_LOOP, 0, 4);
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
