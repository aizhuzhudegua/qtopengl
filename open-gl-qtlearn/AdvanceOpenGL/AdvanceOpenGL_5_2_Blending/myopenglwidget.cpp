#include "myopenglwidget.h"
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QOpenGLFunctions>
#include <QVector>
#include <QKeyEvent>
#include <QMouseEvent> 
#include "vertices.h"

// 玻璃窗口的集合
std::vector<QVector3D> windows;
// map 的key会自动进行排序所以使用它进行玻璃窗口的存储
std::map<float, QVector3D> sorted;

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


    // 初始化草的位置
    windows.push_back(QVector3D(-1.5f, 0.0f, -0.48f));
    windows.push_back(QVector3D( 1.5f, 0.0f, 0.51f));
    windows.push_back(QVector3D( 0.0f, 0.0f, 0.7f));
    windows.push_back(QVector3D(-0.3f, 0.0f, -2.3f));
    windows.push_back(QVector3D( 0.5f, 0.0f, -0.6f));

    // 处理玻璃窗口的绘制顺序
    foreach(auto item,windows) {
        float distance = m_camera.m_cameraPos.distanceToPoint(item);
        sorted[distance] = item;
    }

}

MyOpenGLWidget::~MyOpenGLWidget()
{
    if (!isValid())
        return;
    makeCurrent();

    // 解除shader program
    m_shaderProgram.release();
    m_shaderProgram.removeAllShaders();


    if (m_planeDiffuseTex != nullptr)
        delete m_planeDiffuseTex;
    if(m_windowTexture != nullptr)
        delete m_windowTexture;
    if(m_windowMesh != nullptr)
        delete m_windowMesh;
    if(m_planeMesh != nullptr)
        delete m_planeMesh;

    for(auto iter=m_models.begin();iter!=m_models.end();iter++){
        ModelInfo *modelInfo=&iter.value();
        if(modelInfo != nullptr)
        {
            delete modelInfo->model;
        }

    }

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

    m_planeDiffuseTex = new QOpenGLTexture(QImage(":/images/images/wall.jpg").mirrored());
    // 玻璃窗口纹理
    m_windowTexture = new QOpenGLTexture(QImage(":/images/images/blending_transparent_window.png"));

    m_planeMesh=processMesh(planeVertices,6,m_planeDiffuseTex->textureId());
    m_windowMesh=processMesh(transparentVertices,6,m_windowTexture->textureId());

    // 开启混合
    glEnable(GL_BLEND);
    // 指定混合方式为 使用源颜色的alpha作为源因子，使用1-alpha(这个是源颜色的alpha分量)作为目标因子
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

    //m_planeMesh->draw(m_shaderProgram);

    // 先绘制不透明的物体
    foreach(auto modelInfo,m_models){
        m_model.setToIdentity();
        m_model.translate(modelInfo.worldPos);

        m_model.rotate(modelInfo.pitch,QVector3D(1.0,0.0,0.0));
        m_model.rotate(modelInfo.yaw,QVector3D(0.0,1.0,0.0));
        m_model.rotate(modelInfo.roll,QVector3D(0.0,0.0,1.0));
        m_shaderProgram.setUniformValue("model", m_model);
        modelInfo.model->draw(m_shaderProgram);
    }

    // 绘制玻璃窗口
    for(std::map<float,QVector3D>::reverse_iterator riter=sorted.rbegin();
        riter!=sorted.rend();riter++){
        m_model.setToIdentity();
        m_model.translate(riter->second);
        m_shaderProgram.setUniformValue("model", m_model);
        m_windowMesh->draw(m_shaderProgram);
    }


    m_shaderProgram.release();
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


