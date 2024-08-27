#ifndef CAMERA_H
#define CAMERA_H
#define PI 3.1415926
#include <math.h>
#include <QVector3D>
#include <QMatrix4x4>
#include "behaviour.h"
// 方向
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 默认值
const float YAW        =-90.0f;
const float PITCH      = 0.0f;
const float SPEED      = 2.5f;
const float SENSITVITY = 0.1f;
const float ZOOM       = 45.0f;

class Camera:public Behaviour
{
public:
    // 相机属性
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    // euler angle
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    void updateCameraVectors()
    {
        Front.setX(cos(Yaw*PI/180)* cos(Pitch*PI/180));
        Front.setY(sin(Pitch*PI/180));
        Front.setZ(sin(Yaw*PI/180)* cos(Pitch*PI/180));
        Front.normalize();
        Right = QVector3D::crossProduct(Front,WorldUp);
        Right.normalize();
        Up = QVector3D::crossProduct(Right,Front);
        Up.normalize();
    }

    Camera(QVector3D positon = QVector3D(0.0f,0.0f,0.0f),QVector3D up = QVector3D(0.0f,1.0f,0.0f),
           float yaw = YAW,float pitch = PITCH):Zoom(ZOOM),MouseSensitivity(SENSITVITY),MovementSpeed(SPEED)
    {
        Position = positon;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    QMatrix4x4 GetViewMatrix()
    {
        QMatrix4x4 theMatrix;
        theMatrix.lookAt(Position,Position+Front,Up);
        return theMatrix;
    }

    void ProcessKeyboard(Camera_Movement direction,float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if(direction == FORWARD)
            Position += Front * velocity;
        if(direction == BACKWARD)
            Position -= Front * velocity;
        if(direction == LEFT)
            Position -= Right * velocity;
        if(direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseMovement(float xoffset,float yoffset)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        if(Pitch > 89.0f) Pitch = 89.0f;
        if(Pitch < -89.0f) Pitch = -89.0f;
        updateCameraVectors();
    }

    void ProcessMouseScroll(float angle)
    {
        if(Zoom >= 1.0f && Zoom <= 75.0f)
            Zoom-= angle;
        if(Zoom <= 1.0f) Zoom = 1.0f;
        if(Zoom >= 75.0f) Zoom = 75.0f;
    }


};

#endif // CAMERA_H
