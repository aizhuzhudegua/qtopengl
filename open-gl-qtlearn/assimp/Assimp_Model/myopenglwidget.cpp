#include "myopenglwidget.h"
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QOpenGLFunctions>
#include <QVector>
#include <QKeyEvent>
#include <QMouseEvent> 



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

    // 解除shader program
    m_shaderProgram.release();
    m_shaderProgram.removeAllShaders();

//    if (nullptr != m_diffuseTexture)
//        delete m_diffuseTexture;
//    if (nullptr != m_specularTexture)
//        delete m_specularTexture;
    doneCurrent();
    if (m_pmodel)
        delete m_pmodel;
}

void MyOpenGLWidget::initializeGL()
{
    /*
     * 初始化OpenGL的函数接口
     * 这个接口不能写在构造函数中，否则会程序运行出错，因为initializeOpenGLFunctions()中会需要窗口句柄，而窗口句柄在构造函数完成之后才会被创建
    */
    initializeOpenGLFunctions();

    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/vertex_shader.vert");
    m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/fragment_shader.frag");
    m_shaderProgram.link();

    m_pmodel= new Model(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>()
                ,"D:/TestCode/QTCode/OpenGL/Chapter03/Assimp_Model/backpack/backpack.obj");
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
        model.rotate(time, 1.0f, 1.0f, 0.5f);
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
        *   m_shaderProgram.setUniformValue("material.specular", 0.5f, 0.5f, 0.5f);
        */
        m_shaderProgram.setUniformValue("material.shininess", 32.0f);

        // 设置光源的环境光强度较弱
        m_shaderProgram.setUniformValue("light.ambient", QVector3D(0.4f, 0.4f, 0.4f));
        // 设置光源的光为较暗
        m_shaderProgram.setUniformValue("light.diffuse", QVector3D(0.9f, 0.9f, 0.9f));
        // 设置镜面光为最大强度强度发光
        m_shaderProgram.setUniformValue("light.specular", QVector3D(1.0,1.0,1.0));
        m_shaderProgram.setUniformValue("light.direction", QVector3D(-0.2f, -1.0f, -0.3f));
        m_shaderProgram.setUniformValue("viewPosition", m_camera.m_cameraPos);

        m_shaderProgram.setUniformValue("model", model);

        m_pmodel->draw(m_shaderProgram);


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

//void MyOpenGLWidget::processMesh()
//{
//    QVector<Vertex> _vertices(36);
//    QVector<unsigned int> _indices;
//    QVector<Texture> _textures;
//    memcpy(&_vertices[0],vertices,sizeof(vertices));
//    for(int i=0;i<36;i++){
//        _indices.push_back(i);
//    }

//    Texture tex;
//    tex.id=m_diffuseTexture->textureId();
//    tex.type="texture_diffuse";
//    _textures.push_back(tex);
//    tex.id=m_specularTexture->textureId();
//    tex.type="texture_specular";
//    _textures.push_back(tex);
//    m_pmesh = new Mesh(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>()
//        ,_vertices,_indices,_textures);
//}
