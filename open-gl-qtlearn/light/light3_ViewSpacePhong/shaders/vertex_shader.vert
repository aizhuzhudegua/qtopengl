#version 330 core
layout (location = 0) in vec3 aPos;
// 法线数据
layout (location = 1) in vec3 aNormal;
// 输出法线数据
out vec3 Normal;
// 输出片段位置
out vec3 FragmentPosition;
// 输出计算后的光源位置
out vec3 LightPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
// 世界坐标系下光源位置
uniform vec3 lightPosition;

void main()
{
    // 转换为世界坐标，因为这里的计算是在世界坐标系中计算的
    // vec4(aNormal,0.0)的0.0是为了法线的位移属性，也可以写作mat3(view * model) * aNormal
    // transpose(inverse(view * model)) 是为了求非等比缩放时法线矩阵
    // 这里乘以view矩阵是将其置于观察空间(摄像机坐标系中计算)，这样观察者的位置始终为(0,0,0)
    Normal = vec3(transpose(inverse(view * model)) * vec4(aNormal,0.0));
    FragmentPosition = vec3(view * model * vec4(aPos,1.0));
    // 因为光源位置本身就在世界坐标系中，所以只用乘以view矩阵即可
    LightPosition = vec3(view * vec4(lightPosition,1.0));
    gl_Position = projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.0f);
}
