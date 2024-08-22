#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>

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

private:
    Shape m_shape;

signals:

};

#endif // OPENGLWIDGET_H
