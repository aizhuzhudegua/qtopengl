#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <memory>
#include <QTimer>
#include <QTime>
#include <QVector3D>
#include <QMouseEvent>
#include <camera.h>
#include <QOpenGLTexture>
#include <QPoint>
#include "model.h"


struct ModelInfo{
    Model *model;
    QVector3D worldPos;
    float pitch;
    float yaw;
    float roll;
    bool isSelected;
    QString name;
};

class MyOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget();

    void loadModel(QString path);
protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
signals:
    // 鼠标点击时发送的信号
    void mousePickingPos(QVector3D wolrdpos);
private: // 成员函数
    QVector3D cameraPosInit(float maxY, float minY);
    Mesh* processMesh(float *vertices, int size,unsigned int textureId);
    // 计算鼠标点击模型的位置
    QVector4D worldPosFromViewPort(int posX,int posY);

private:
    // shader program
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLShaderProgram m_postProcessingShaderProgram;

    // 旋转定时器
    QTimer m_rotateTimer;
    // 超时处理函数
    void timeOutHandle();
    QTime m_time;
    // 相机位置
    QVector3D m_cameraPos;
    // 相机看向的方向
    QVector3D m_cameraLookAtFrontDirection;
    // 相机的右方向
    QVector3D m_cameraRightDirection;
    /*
    * projection矩阵中的fov角度
    *   fov(field of View) 定义了可以看到场景中多大的范围。当视野变小时，场景投影出来的空间就会减小，
    * 产生放大(Zoom In)的感觉
    */ 
    float m_fov;
    Camera m_camera;

    // 相机的初始位置
    QVector3D m_cameraInitPos;

    // 地面纹理
    QOpenGLTexture * m_planeDiffuseTex;
    // 玻璃窗口纹理
    QOpenGLTexture * m_windowTexture;

    // 近平面
    float m_near;
    // 远平面
    float m_far;

    QMap<QString, ModelInfo> m_models;
    Mesh * m_planeMesh;
    Mesh * m_windowMesh;

    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_projection;

    // 是否处于移动状态
    bool m_modelMoving;
    // 鼠标点击时的位置
    QPoint m_lastPos;

    /*
     * 帧缓冲相关变量
     * fbo 帧缓冲对象
     * rbo 渲染缓冲Attachment
     * texture 纹理 Attachment
    */
    GLuint m_fbo;
    GLuint m_rbo;
    GLuint m_texture;
};

#endif // MYOPENGLWIDGET_H
