 #include "openglwidget.h"

#define TIMEOUTMSFC 100
#define PI 3.1415926

OpenGlWidget::OpenGlWidget(QWidget *parent) : QOpenGLWidget(parent),QOpenGLFunctions_4_5_Core()
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    connect(&timer,SIGNAL(timeout()),
            this,SLOT(onTimeout()));
    timer.start(TIMEOUTMSFC);
    m_time.start();
    cameraFront = QVector3D(0.0,0.0,-1.0);

    cameraPos = QVector3D(0.0f,0.0f,3.0f);
    up = QVector3D(0.0f ,1.0f, 0.0f);
    cameraRight = QVector3D::crossProduct(up,cameraFront);
    cameraRight.normalize();
    lastFrame = 0;
    fov = 45;
}


unsigned int VBO,VAO,EBO;
float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,
};

QVector<QVector3D> cubePositions = {
  QVector3D(0.0f,  0.0f,  0.0f),
  QVector3D(2.0f,  5.0f, -15.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-3.8f, -2.0f, -12.3f),
  QVector3D(2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
  QVector3D(1.3f, -2.0f, -2.5f),
  QVector3D(1.5f,  2.0f, -2.5f),
  QVector3D(1.5f,  0.2f, -1.5f),
  QVector3D(-1.3f,  1.0f, -1.5f)
};

float ratio = 0.5;

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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof (float),(void*)0);
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof (float),(void*)(sizeof(float)*3));
    // 开启VAO的第一个属性值
    glEnableVertexAttribArray(1);





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
    texSmall = new QOpenGLTexture(QImage(":texture/texture/3.png").mirrored());

    shaderProgram.bind();
    shaderProgram.setUniformValue("texture0",0);
    shaderProgram.setUniformValue("texture1",1);
    shaderProgram.setUniformValue("texSmall",2);
    // 释放助理
    glBindVertexArray(0);

    // 线框模式
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);


    shaderProgram.bind();


    shaderProgram.setUniformValue("ratio",ratio);
}

void OpenGlWidget::resizeGL(int w, int h)
{

}

float angle = 0;
#include <math.h>

void OpenGlWidget::paintGL()
{

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    projection.perspective(fov,(float)width()/height(),0.1,100);
//    float time = m_time.elapsed() / 1000.0;
//    const float distance = 10.0f;
//    float camX = sin(time) * distance;
//    float camZ = cos(time) * distance;

    // 这里相当于相机的位置在 （0，0，3）
    // view.translate(0.0,0.0,-3);

    // QVector3D up = QVector3D(0.0f,1.0f,0.0f);


//    QMatrix4x4 trans;
//    trans.translate(QVector3D(-cameraPos.x(),-cameraPos.y(),-cameraPos.z()));
//    QMatrix4x4 view(
//                cameraRight.x(), cameraRight.y(), cameraRight.z(),0,
//                cameraUp.x(), cameraUp.y(), cameraUp.z(),0,
//                cameraDirection.x(), cameraDirection.y(), cameraDirection.z(),0,
//                0, 0, 0, 1
//                    );

//    view = view*trans;

    view.lookAt(cameraPos,cameraPos+cameraFront,up);

    // set state
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    // 开启深度测试
    glEnable(GL_DEPTH_TEST);
    // use state
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS);

    shaderProgram.bind();
    shaderProgram.setUniformValue("projection",projection);
    shaderProgram.setUniformValue("view",view);
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
            texSmall->bind(2);

            // 生成多级纹理
            texSmall->generateMipMaps();

            float borderColor[] = {1.0f,1.0f,0.0};
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
            glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

//            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

            foreach(auto item,cubePositions)
            {
                 model.setToIdentity();

                 // 先平移后旋转
                 model.translate(item);
                 // model.rotate(angle,1.0f,1.0f,1.0f);

                 shaderProgram.setUniformValue("model",model);
                 glDrawArrays(GL_TRIANGLES,0,36);
            }
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

void OpenGlWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = 2.5*TIMEOUTMSFC/1000.0;

    switch(event->key()){
        case Qt::Key_Up:
        {

            ratio+=0.1;
            break;
        }
        case Qt::Key_Down:
        {

            ratio-=0.1;
            break;
        }
        case Qt::Key_W:
        {

            cameraPos+=cameraFront * cameraSpeed;
            break;
        }

        case Qt::Key_S:
        {

            cameraPos-=cameraFront * cameraSpeed;
            break;
        }

        case Qt::Key_A:
        {

            cameraPos+=cameraRight * cameraSpeed;
            break;
        }

        case Qt::Key_D:
        {
            cameraPos-=cameraRight * cameraSpeed;
            break;
        }


    }
    if(ratio > 1 )ratio = 1;
    if(ratio < 0 )ratio = 0;
    qDebug() << ratio;
    makeCurrent();
    shaderProgram.bind();
    shaderProgram.setUniformValue("ratio",ratio);
    doneCurrent();
    update();
}

void OpenGlWidget::mouseMoveEvent(QMouseEvent *event)
{
    static float yaw=-90;
    static float pitch = 0;
    static QPoint lastPos(width()/2.0,height()/2.0);
    auto currentPos = event->pos();
    deltaPos = currentPos-lastPos;
    lastPos = currentPos;
    float sensitivity = 0.1f;
    deltaPos*=sensitivity;
    yaw += deltaPos.x();
    pitch -= deltaPos.y();

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    cameraFront.setX(cos(yaw*PI/180)* cos(pitch*PI/180));
    cameraFront.setY(sin(pitch*PI/180));
    cameraFront.setZ(sin(yaw*PI/180)* cos(pitch*PI/180));
    cameraFront.normalize();
    update();

}

void OpenGlWidget::wheelEvent(QWheelEvent *event)
{
    if(fov >= 1.0f && fov <= 75.0f)
        fov-= event->angleDelta().y()/120;
    if(fov <= 1.0f) fov = 1.0f;
    if(fov >= 75.0f) fov = 75.0f;
}

void OpenGlWidget::onTimeout()
{

    update();
}
