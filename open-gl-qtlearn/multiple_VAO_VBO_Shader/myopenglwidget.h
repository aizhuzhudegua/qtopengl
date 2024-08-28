#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

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
        Wireframe
    };

    void drawShape(Shape shape);

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
signals:

private:
    GLuint m_VBO[2] = {0};
    GLuint m_VAO[2] = {0};

    // 这里使用非静态变量报错，目前还不清楚具体的是为什么(也可能是我环境本身的问题)
    static GLuint m_EBO;
    
    // 表示绘制的类型
    Shape m_shape;

    // shader program
    QOpenGLShaderProgram m_shaderProgram;
    QOpenGLShaderProgram m_shaderProgram1;

};

#endif // MYOPENGLWIDGET_H
