#include "myopenglwidget.h"
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QOpenGLFunctions>
#include <QVector>
#include <QKeyEvent>
#include <QMouseEvent> 

GLuint MyOpenGLWidget::m_EBO = 0;

// 环绕方式使用
float vertices[] = {
    // positions          // colors              
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

GLuint index[] = {
    0,1,3,
    1,2,3
};

QVector<QVector3D> cubePositions = {
    QVector3D(0.0f, 0.0f, 0.0f),
    QVector3D(2.0f, 5.0f, -15.0f),
    QVector3D(-1.5f, -2.2f, -2.5f),
    QVector3D(-3.8f, -2.0f, -12.3f),
    QVector3D(2.4f, -0.4f, -3.5f),
    QVector3D(-1.7f, 3.0f, -7.5f),
    QVector3D(1.3f, -2.0f, -2.5f),
    QVector3D(1.5f, 2.0f, -2.5f),
    QVector3D(1.5f, 0.2f, -1.5f),
    QVector3D(-1.3f, 1.0f, -1.5f)
};


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    m_shape(None)
{
    // 设置焦点
    setFocusPolicy(Qt::StrongFocus);
    // 开启鼠标追踪,关闭时只有鼠标按下时移动鼠标图形才会转动
    //setMouseTracking(true);

    connect(&m_rotateTimer, &QTimer::timeout, this, &MyOpenGLWidget::timeOutHandle);
    m_rotateTimer.start(100);
    m_time.start();

    m_camera.m_cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
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
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,5 * sizeof(GLfloat),(void*)0);
    // 开启VAO管理的第一个属性值
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    // 开启VAO管理的第二个属性值
    glEnableVertexAttribArray(1);

    
    
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
    //m_texture = std::make_unique<QOpenGLTexture>(QImage(":/images/images/small.png").mirrored());
    m_texture = std::make_unique<QOpenGLTexture>(QImage(":/images/images/wall.jpg").mirrored());
    

    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("texture", 0);

    m_shaderProgram.release();
    


}

void MyOpenGLWidget::paintGL()
{
    // 设置Clear的属性
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    // 使用
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0.0, 0, width(), height());


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
        //float time = m_time.elapsed()/1000.0;
        //float radius = 10.f;
        //float camX = sin(time) * radius;
        //float camZ = cos(time) * radius;
        //view.lookAt(QVector3D(camX, 0.0f, camZ), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
        
        // 摄像机一直看向原点，不随左右移动而调整lookat方向(但是调整摄像机本身的位置)，相当于绕着原点旋转摄像机
        //view.lookAt(m_cameraPos, QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
        // 摄像机跟随左右移动而调整lookat方向，相当于左右平移摄像机
        //view.lookAt(m_cameraPos, m_cameraPos + QVector3D(0.0f, 0.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f));

        // 绑定shader
        m_shaderProgram.bind();
        m_texture->bind();
        
        m_shaderProgram.setUniformValue("view", view);

        QMatrix4x4 projection;
        projection.perspective(m_camera.m_zoom, (float)width() / height(), 0.1, 100.0);
        m_shaderProgram.setUniformValue("projection", projection);


        foreach(auto item, cubePositions)
        {
            // 重置model矩阵
            model.setToIdentity();
            model.translate(item);
            m_shaderProgram.setUniformValue("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        m_texture->release();
        m_shaderProgram.release();
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
    //update();
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
    default:
        break;
    }
    update();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) 
{
    static QPoint lastPos(width() / 2, height() / 2);
    auto currentPos = event->pos();
    auto deltaPos = currentPos - lastPos;
    lastPos = currentPos;

    m_camera.ProcessMouseMovement(deltaPos.x(), -deltaPos.y());
    update();
}

void MyOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    m_camera.ProcessMouseScroll(event->angleDelta().y() / 120);
    update();
}