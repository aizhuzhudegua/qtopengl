#include "myopenglwidget.h"
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QOpenGLFunctions>
#include <QVector>
#include <QKeyEvent>
#include <QMouseEvent> 
#include "vertices.h"



MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent),
    m_cameraInitPos(0.0f, 5.0f, 20.0f),
    m_near(0.1f),
    m_far(100.0f),
    m_planeMesh(nullptr),
    m_modelMoving(false)
{
    // 设置焦点
    setFocusPolicy(Qt::StrongFocus);
    // 开启鼠标追踪,关闭时只有鼠标按下时移动鼠标图形才会转动
    //setMouseTracking(true);

    connect(&m_rotateTimer, &QTimer::timeout, this, &MyOpenGLWidget::timeOutHandle);
    m_rotateTimer.start(50);
    m_time.start();

    m_camera.m_cameraPos = m_cameraInitPos;
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    if (!isValid())
        return;
    makeCurrent();

    // 解除shader program
    m_shaderProgram.release();
    m_shaderProgram.removeAllShaders();

    m_skyBoxShaderProgram.release();
    m_skyBoxShaderProgram.removeAllShaders();

    if(m_cubeMap != nullptr)
        delete m_cubeMap;
    if (m_planeDiffuseTex != nullptr)
        delete m_planeDiffuseTex;
    if(m_planeMesh != nullptr)
        delete m_planeMesh;

    for(auto iter=m_models.begin();iter!=m_models.end();iter++){
        ModelInfo *modelInfo=&iter.value();
        if(modelInfo != nullptr)
        {
            delete modelInfo->model;
        }

    }

    glDeleteVertexArrays(1,&m_skyboxVAO);
    glDeleteBuffers(1,&m_skyboxVBO);
    doneCurrent();
}

void MyOpenGLWidget::loadModel(QString path)
{
    static int i=0;
    makeCurrent();
    Model* pmodel= new Model(QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>()
                ,path);
    QString key = "models"+QString::number(i++);
    // 减去m_minY是为了将model放到地平面以上
    m_models[key] = ModelInfo{pmodel,QVector3D(0,0 - pmodel->m_minY,0) ,0.0,0.0,0.0,false,key};
    doneCurrent();
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

    m_planeDiffuseTex=new QOpenGLTexture(QImage(":/images/images/wall.jpg").mirrored());

    m_planeMesh=processMesh(planeVertices,6,m_planeDiffuseTex->textureId());


    m_skyBoxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/shaders/skybox.vert");
    m_skyBoxShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/shaders/skybox.frag");
    m_skyBoxShaderProgram.link();
    // 创建立方体纹理
    m_cubeMap=new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);
    // 加载纹理图片
    QImage _right   = QImage(":/images/images/skybox/right.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _left    = QImage(":/images/images/skybox/left.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _top     = QImage(":/images/images/skybox/top.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _bottom  = QImage(":/images/images/skybox/bottom.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _front   = QImage(":/images/images/skybox/front.jpg").convertToFormat(QImage::Format_RGB888);
    QImage _back    = QImage(":/images/images/skybox/back.jpg").convertToFormat(QImage::Format_RGB888);
    // 设置宽高
    m_cubeMap->setSize(_right.width(), _right.height());
    // 设置颜色模式
    m_cubeMap->setFormat(QOpenGLTexture::RGBFormat);
    // 分配存储区域
    m_cubeMap->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8);
    // 分别设置六个面的纹理数据
    m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveX,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_right.bits());
    m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeX,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_left.bits());
    m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveY,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_top.bits());
    m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeY,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_bottom.bits());
    m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_front.bits());
    m_cubeMap->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)_back.bits());

    //纹理放大或缩小时，像素的取值方法 ，线性或就近抉择
    m_cubeMap->setMinificationFilter(QOpenGLTexture::Linear);
    m_cubeMap->setMagnificationFilter(QOpenGLTexture::Linear);
    //设置纹理边缘的扩展方法
    m_cubeMap->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    m_cubeMap->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

    // skybox VAO
    glGenVertexArrays(1, &m_skyboxVAO);
    glGenBuffers(1, &m_skyboxVBO);
    glBindVertexArray(m_skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MyOpenGLWidget::paintGL()
{

    // 设置Clear的属性
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    // 使用
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_model.setToIdentity();
    m_view.setToIdentity();
    m_projection.setToIdentity();

    m_view = m_camera.GetViewMatrix(); ;
    // 绑定shader
    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("view", m_view);


    m_projection.perspective(m_camera.m_zoom, (float)width() / height(), m_near, m_far);
    m_shaderProgram.setUniformValue("projection", m_projection);

    m_shaderProgram.setUniformValue("model", m_model);
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
    //qDebug() << m_camera.m_cameraPos;

    m_planeMesh->draw(m_shaderProgram);

    // 指定纹理单元索引
    m_shaderProgram.setUniformValue("skybox", 4);
    // 绑定立方体纹理
    m_cubeMap->bind(4);

    foreach(auto modelInfo,m_models){
        m_model.setToIdentity();
        m_model.translate(modelInfo.worldPos);

        m_model.rotate(modelInfo.pitch,QVector3D(1.0,0.0,0.0));
        m_model.rotate(modelInfo.yaw,QVector3D(0.0,1.0,0.0));
        m_model.rotate(modelInfo.roll,QVector3D(0.0,0.0,1.0));
        m_shaderProgram.setUniformValue("model", m_model);
        modelInfo.model->draw(m_shaderProgram);
    }

    m_shaderProgram.release();

    // 深度测试函数确保天空盒等于1也能通过测试
    glDepthFunc(GL_LEQUAL);
    m_skyBoxShaderProgram.bind();
    QMatrix4x4 skyboxView=m_view;
    // 移除skyboxView的位移属性
    skyboxView.setColumn(3,QVector4D(0.0f,0.0f,0.0f,1.0f));
    m_skyBoxShaderProgram.setUniformValue("projection", m_projection);
    m_skyBoxShaderProgram.setUniformValue("view", skyboxView);
    // 指定纹理单元索引
    m_skyBoxShaderProgram.setUniformValue("skybox", 0);
    // 绑定立方体纹理
    m_cubeMap->bind(0);
    // 激活VAO
    glBindVertexArray(m_skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    // 深度测试函数设置为默认行为
    glDepthFunc(GL_LESS);
    m_skyBoxShaderProgram.release();
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
    case Qt::Key_Space:m_camera.m_cameraPos = m_cameraInitPos;break;
    default:
        break;
    }
    update();
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* event) 
{
    if(m_modelMoving){
        for(auto iter=m_models.begin();iter!=m_models.end();iter++){
            ModelInfo *modelInfo=&iter.value();
            if(!modelInfo->isSelected)
                continue;
            // 计算移动后 model 的位置
            modelInfo->worldPos = QVector3D(worldPosFromViewPort(event->pos().x(),event->pos().y()));
        }
    }
    else
    {
        if(event->buttons() & Qt::RightButton
                || event->buttons() & Qt::LeftButton
                || event->buttons() & Qt::MiddleButton){

            auto currentPos=event->pos();
            QPoint deltaPos=currentPos-m_lastPos;
            m_lastPos=currentPos;
            if(event->buttons() & Qt::RightButton)
            {
                m_camera.ProcessMouseMovement(deltaPos.x(),-deltaPos.y());
            }
            else
            {
                for(auto iter=m_models.begin();iter!=m_models.end();iter++)
                {
                    ModelInfo *modelInfo=&iter.value();
                    if(!modelInfo->isSelected)
                        continue;
                    if(event->buttons() & Qt::MiddleButton)
                    {
                        // 绕 z 轴旋转
                        modelInfo->roll+=deltaPos.x();
                    }
                    else if(event->buttons() & Qt::LeftButton)
                    {
                        // 绕 y 轴旋转
                        modelInfo->yaw+=deltaPos.x();
                        // 绕 x 轴旋转
                        modelInfo->pitch+=deltaPos.y();
                    }
                }
            }

        }
    }

}

void MyOpenGLWidget::wheelEvent(QWheelEvent* event)
{
    m_camera.ProcessMouseScroll(event->angleDelta().y() / 120);
    update();
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    bool hasSelected=false;
    makeCurrent();
    m_lastPos=event->pos();

    if(event->buttons()&Qt::LeftButton)
    {
        QVector4D wolrdPostion=worldPosFromViewPort(event->pos().x(),event->pos().y());
        emit mousePickingPos(QVector3D(wolrdPostion));

        // 遍历所有 model
        for(QMap<QString, ModelInfo>::iterator iter=m_models.begin();iter!=m_models.end();iter++)
        {
            ModelInfo *modelInfo=&iter.value();
            // 计算 model 的包络半径，若点击在这个包络求中则认为选中
            float r=(modelInfo->model->m_maxY-modelInfo->model->m_minY)/2;
            if(modelInfo->worldPos.distanceToPoint(QVector3D(wolrdPostion))<r
                    &&!hasSelected){
                modelInfo->isSelected=true;
                hasSelected=true;
            }
            else
            {
                modelInfo->isSelected=false;
            }
            qDebug()<<modelInfo->worldPos.distanceToPoint(QVector3D(wolrdPostion))<<"<"<<r<<"="<<modelInfo->isSelected;
        }
    }
}

void MyOpenGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(m_modelMoving){
        //再次双击取消移动
        m_modelMoving=false;
    }else
    {
        foreach(auto modelInfo,m_models){
            //双击启动移动
            if(modelInfo.isSelected==true)
                m_modelMoving=true;
            else
                continue;
            qDebug()<<modelInfo.name<<modelInfo.isSelected;
        }
    }
}

QVector3D MyOpenGLWidget::cameraPosInit(float maxY, float minY)
{
    QVector3D temp={0,0,0};
    float height=maxY-minY;
    temp.setZ(1.5*height);
    if(minY>=0)
        temp.setY(height/2.0);
    m_cameraInitPos = temp;
    return temp;
}

Mesh *MyOpenGLWidget::processMesh(float *vertices, int size, unsigned int textureId)
{
    QVector<Vertex> _vertices;
    QVector<unsigned int> _indices;
    std::vector<Texture> _textures;

    for(int i=0;i<size;i++){
        Vertex vert;
        vert.Position[0]=vertices[i*5+0];
        vert.Position[1]=vertices[i*5+1];
        vert.Position[2]=vertices[i*5+2];
        vert.TexCoords[0]=vertices[i*5+3];
        vert.TexCoords[1]=vertices[i*5+4];
        _vertices.push_back(vert);
        _indices.push_back(i);
    }
    Texture tex; tex.id=textureId;
    tex.type="texture_diffuse";
    _textures.push_back(tex);
    return new Mesh(
                QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>()
                ,_vertices,_indices,_textures);
}

QVector4D MyOpenGLWidget::worldPosFromViewPort(int posX, int posY)
{
    // 深度缓冲中的深度值
    float depthZ;
    makeCurrent();
    /*
     * 从深度缓冲中获取鼠标点击时对应的深度值
     * this->height()-posY 是因为opengl 视窗中 y 坐标中的原点在左下角且正方向向上，而屏幕坐标系的y轴原点在左上角且正方向朝下
    */
    glReadPixels(posX,this->height()-posY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depthZ);
    doneCurrent();
    // 这里的 (x,y,z)的计算是将其归一化到opengl的标准坐标中即[-1,1]之间
    float x=(2.0f*posX)/this->width()-1.0f;
    float y=1.0f-(2.0f*posY)/this->height();
    float z=depthZ*2.0-1.0f;

    // opengl 官网(https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/01%20Depth%20testing/)提供的方式
    //float w = (2.0 * m_near * m_far) / (m_far + m_near - z * (m_far - m_near));
    // 根据官网depth线性计算和非线性计算公式，推导出的非线性depth转换为线性深度的方式
    float w= m_near*m_far/(m_near*depthZ-m_far*depthZ+m_far);
    // 世界坐标位置
    QVector4D wolrdPostion(x,y,z,1);
    // 转换为投影矩阵下未进行透视投影处理时的坐标
    wolrdPostion=w*wolrdPostion;
    // 转换成世界坐标位置
    return m_view.inverted()*m_projection.inverted()*wolrdPostion;
}


