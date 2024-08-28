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
#include "mesh.h"

class MyOpenGLWidget : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget();
    enum Shape
    {
        None,
        Rect,
        Circle,
        Triangle,
        RectWireframe
    };

    void drawShape(Shape shape);

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
signals:
private: // 方法
    void processMesh();
private:

    // 表示绘制的类型
    Shape m_shape;

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

    // 漫反射纹理贴图
    QOpenGLTexture* m_diffuseTexture;
    // 镜面光照纹理贴图
    QOpenGLTexture* m_specularTexture;

    Mesh* m_pmesh;
};

#endif // MYOPENGLWIDGET_H
