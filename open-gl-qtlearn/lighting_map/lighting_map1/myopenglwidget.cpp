#include "myopenglwidget.h"
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QOpenGLFunctions>
#include <QVector>
#include <QKeyEvent>
#include <QMouseEvent> 

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};



MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    m_shape(Rect)
{
    // 设置焦点
    setFocusPolicy(Qt::StrongFocus);
    // 开启鼠标追踪,关闭时只有鼠标按下时移动鼠标图形才会转动
    //setMouseTracking(true);

    connect(&m_rotateTimer, &QTimer::timeout, this, &MyOpenGLWidget::timeOutHandle);
    m_rotateTimer.start(50);
    m_time.start();

    m_camera.m_cameraPos = QVector3D(0.0f, 0.0f, 5.0f);
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    if (!isValid())
        return;
    makeCurrent();
    // 解除VBO
    glDeleteBuffers(1,&m_VBO);
    // 解除VAO
    glDeleteVertexArrays(1,&m_VAO);
    glDeleteVertexArrays(1, &m_lightVAO);
    // 解除shader program
    m_shaderProgram.release();
    m_shaderProgram.removeAllShaders();
    m_lightShaderProgram.release();
    m_lightShaderProgram.removeAllShaders();
    if (nullptr != m_diffuseTexture)
        delete m_diffuseTexture;
    if (nullptr != m_specularTexture)
        delete m_specularTexture;
    if (nullptr != m_emissionTexture)
        delete m_emissionTexture;
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
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3*sizeof(float)));
    // 开启VAO管理的第二个属性值
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float)));
    // 开启VAO管理的第三个属性值
    glEnableVertexAttribArray(2);


    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);
    // 使用已有的VBO,此VBO中以关联数据
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/vertex_shader.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/fragment_shader.frag");
    m_shaderProgram.link();

    m_diffuseTexture = new QOpenGLTexture(QImage(":/images/images/diffuse_map.png").mirrored());
    m_specularTexture = new QOpenGLTexture(QImage(":/images/images/specular_map.png").mirrored());
    // 边框为彩色的镜面光贴图
    //m_specularTexture = new QOpenGLTexture(QImage(":/images/images/specular_map_color.png").mirrored());
    // 放射光贴图
    m_emissionTexture = new QOpenGLTexture(QImage(":/images/images/emissions_map.jpg").mirrored());
    m_shaderProgram.bind();
    // 指定纹理单元索引
    m_shaderProgram.setUniformValue("material.diffuse", 0);
    m_shaderProgram.setUniformValue("material.specular", 1);
    m_shaderProgram.setUniformValue("material.emission", 2);



    m_lightShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/light.vert");
    m_lightShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/light.frag");
    m_lightShaderProgram.link();
    m_lightShaderProgram.bind();
    m_lightShaderProgram.setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    m_lightShaderProgram.release();


}

void MyOpenGLWidget::paintGL()
{
    // 设置Clear的属性
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    // 使用
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // 绘制图形
    //
    switch (m_shape)
    {
    case MyOpenGLWidget::None:
        break;
    case MyOpenGLWidget::Rect:
    {
        QMatrix4x4 view = m_camera.GetViewMatrix(); ;
        QMatrix4x4 model;
        float time = m_time.elapsed()/50.0;
        model.rotate(time, 1.0f, 4.0f, 0.5f);
        // 绑定shader
        m_shaderProgram.bind();
        
        m_shaderProgram.setUniformValue("view", view);

        QMatrix4x4 projection;
        projection.perspective(m_camera.m_zoom, (float)width() / height(), 0.1, 100.0);
        m_shaderProgram.setUniformValue("projection", projection);

        m_shaderProgram.setUniformValue("model", model);
        /*
        *   m_shaderProgram.setUniformValue("material.ambient", 1.0f, 0.5f, 0.31f);
        *   m_shaderProgram.setUniformValue("material.diffuse", 1.0f, 0.5f, 0.31f);
        *   这两部分被整合到了漫反射贴图中了
        */
        m_shaderProgram.setUniformValue("material.specular", 0.5f, 0.5f, 0.5f);
        m_shaderProgram.setUniformValue("material.shininess", 32.0f);

        // 设置光源的环境光强度较弱
        m_shaderProgram.setUniformValue("light.ambient", QVector3D(0.2f, 0.2f, 0.2f));
        // 设置光源的光为较暗
        m_shaderProgram.setUniformValue("light.diffuse", QVector3D(0.5f, 0.5f, 0.5f));
        // 设置镜面光为最大强度强度发光
        m_shaderProgram.setUniformValue("light.specular", QVector3D(1.0,1.0,1.0));

        QVector3D lightPosition(1.2f, 1.0f, 2.0f);
        lightPosition.setX(cos(time / 50)*1.5);
        lightPosition.setY(sin(time / 50)*1.2);
        m_shaderProgram.setUniformValue("light.position", lightPosition);
        m_shaderProgram.setUniformValue("viewPosition", m_camera.m_cameraPos);

        // 绘制前绑定纹理单元
        m_diffuseTexture->bind(0);
        m_specularTexture->bind(1);
        m_emissionTexture->bind(2);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        m_shaderProgram.release();

        m_lightShaderProgram.bind();
        m_lightShaderProgram.setUniformValue("projection", projection);
        m_lightShaderProgram.setUniformValue("view", view);
        model.setToIdentity();
        model.translate(lightPosition);
        model.rotate(1.0, 1.0f, 5.0f, 0.5f);
        model.scale(0.2);
        m_lightShaderProgram.setUniformValue("model", model);
        glBindVertexArray(m_lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        m_lightShaderProgram.release();
        break;
    }
    case MyOpenGLWidget::Circle:
        break;
    case MyOpenGLWidget::Triangle:
        break;
    case MyOpenGLWidget::RectWireframe:
    {
        glDrawArrays(GL_LINE_LOOP, 0, 36);
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

void MyOpenGLWidget::timeOutHandle()
{
    update();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent* event)
{
    // deltaTime 用来模拟每一帧刷新之间的时间间隔，这里设置为与m_timer的超时时间一直
    float deltaTime = 100.0f / 1000.0f;
    switch (event->key())
    {
    // QVector3D(0.0f,0.0f,1.0f) 表示前进的步长
    case Qt::Key_W:m_camera.ProcessKeyboard(Camera::FORWARD, deltaTime);break;
    case Qt::Key_S:m_camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);break;
        // QVector3D(1.0f, 0.0f, 0.0f) 表示左右的步长
    case Qt::Key_D:m_camera.ProcessKeyboard(Camera::RIGHT, deltaTime);break;
    case Qt::Key_A:m_camera.ProcessKeyboard(Camera::LEFT, deltaTime);break;
    case Qt::Key_Q:m_camera.ProcessKeyboard(Camera::DOWN, deltaTime);break;
    case Qt::Key_E:m_camera.ProcessKeyboard(Camera::UP, deltaTime);break;
        // 恢复初始位置
    case Qt::Key_Space:m_camera.m_cameraPos = QVector3D(0.0f, 0.0f, 5.0f);break;
    default:
        break;
    }
    update();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) 
{
    if(event->buttons() & Qt::LeftButton)
    {
        static QPoint lastPos(width() / 2, height() / 2);
        auto currentPos = event->pos();
        auto deltaPos = currentPos - lastPos;
        lastPos = currentPos;

        m_camera.ProcessMouseMovement(deltaPos.x(), -deltaPos.y());
        update();
    }

}

void MyOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    m_camera.ProcessMouseScroll(event->angleDelta().y() / 120);
    update();
}
