 #include "openglwidget.h"

#define TIMEOUTMSFC 100
#define PI 3.1415926

#include "material.h"

OpenGlWidget::OpenGlWidget(QWidget *parent) : QOpenGLWidget(parent),QOpenGLFunctions_4_5_Core()
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    connect(&timer,SIGNAL(timeout()),
            this,SLOT(onTimeout()));
    timer.start(TIMEOUTMSFC);
    m_time.start();

    camera.Position = QVector3D(0.0f,0.0f,3.0f);

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



    unsigned int EBO;
    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof (indices),indices,GL_STATIC_DRAW);


    Material material;
    material.setBaseMap(QImage(":texture/texture/1.jpg").mirrored());
    material.setShader(":/shaders/shapes.vert",":/shaders/shapes.frag");
    material.use();
//    shaderProgram.bind();
//    shaderProgram.setUniformValue("texture0",0);
//    shaderProgram.setUniformValue("texture1",1);
//    shaderProgram.setUniformValue("texSmall",2);
    // 释放助理
    glBindVertexArray(0);


//    shaderProgram.bind();


//    shaderProgram.setUniformValue("ratio",ratio);
}

void OpenGlWidget::resizeGL(int w, int h)
{

}

float angle = 0;


void OpenGlWidget::paintGL()
{

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;
    projection.perspective(camera.Zoom,(float)width()/height(),0.1,100);

    view = camera.GetViewMatrix();

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
//            texture->bind(0);
//            texture1->bind(1);
//            texSmall->bind(2);

            // 生成多级纹理
//            texSmall->generateMipMaps();

//            float borderColor[] = {1.0f,1.0f,0.0};
//            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
//            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
//            glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

//            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
//            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

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

            camera.ProcessKeyboard(FORWARD,TIMEOUTMSFC/1000.0);
            break;
        }

        case Qt::Key_S:
        {

            camera.ProcessKeyboard(BACKWARD,TIMEOUTMSFC/1000.0);
            break;
        }

        case Qt::Key_A:
        {

            camera.ProcessKeyboard(LEFT,TIMEOUTMSFC/1000.0);
            break;
        }

        case Qt::Key_D:
        {
            camera.ProcessKeyboard(RIGHT,TIMEOUTMSFC/1000.0);
            break;
        }


    }
    if(ratio > 1 )ratio = 1;
    if(ratio < 0 )ratio = 0;

    makeCurrent();
    shaderProgram.bind();
    shaderProgram.setUniformValue("ratio",ratio);
    doneCurrent();

}

void OpenGlWidget::mouseMoveEvent(QMouseEvent *event)
{

    static QPoint lastPos(width()/2.0,height()/2.0);
    auto currentPos = event->pos();
    QPoint deltaPos = currentPos-lastPos;
    lastPos = currentPos;

    camera.ProcessMouseMovement( deltaPos.x(), -deltaPos.y());

}

void OpenGlWidget::wheelEvent(QWheelEvent *event)
{
    camera.ProcessMouseScroll(event->angleDelta().y()/120);

}

void OpenGlWidget::onTimeout()
{

    update();
}
