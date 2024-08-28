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
#include "model.h"

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
signals:
private: // 成员函数
    QVector3D cameraPosInit(float maxY, float minY);
    Mesh* processMesh(float *vertices, int size,unsigned int textureId);
private:
    // shader program
    QOpenGLShaderProgram m_shaderProgram;

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

    // 盒子纹理
    QOpenGLTexture * m_boxDiffuseTex;
    // 地面纹理
    QOpenGLTexture * m_planeDiffuseTex;


    Model* m_pmodel;
    Mesh * m_cubeMesh;
    Mesh * m_planeMesh;
};

#endif // MYOPENGLWIDGET_H
