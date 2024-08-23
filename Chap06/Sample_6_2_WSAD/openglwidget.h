#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>

class OpenGlWidget : public QOpenGLWidget,QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit OpenGlWidget(QWidget *parent = nullptr);
    ~OpenGlWidget();
    enum Shape{None,Rect,Circle,Triangle};
    void drawShape(Shape Shape);
    void setWireFrame(bool mode);

// QOpenGLWidget类需要重载三个函数
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);

public slots:
    void onTimeout();

private:
    Shape m_shape;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLTexture *texture;
    QOpenGLTexture *texture1;
    QOpenGLTexture *texSmall;
    QTimer timer;
    QTime m_time;

    QVector3D cameraPos;

    QVector3D cameraDirection;
    QVector3D cameraRight;
    QVector3D cameraUp;

    QVector3D cameraTarget;
    QVector3D cameraFront;

    QVector3D up;
    float deltaTime;
    float lastFrame;
    QPoint deltaPos;

    float fov;


signals:

};

#endif // OPENGLWIDGET_H
