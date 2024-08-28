#version 330 core
layout (location = 0) in vec3 aPos;
// 法线数据
layout (location = 1) in vec3 aNormal;
// 输出法线数据
out vec3 Normal;
// 输出片段位置
out vec3 FragmentPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    // 转换为世界坐标，因为这里的计算是在世界坐标系中计算的
    // vec4(aNormal,0.0)的0.0是为了法线的位移属性，也可以写作mat3(model) * aNormal
    // transpose(inverse(model)) 是为了求非等比缩放时法线矩阵
    Normal = vec3(transpose(inverse(model)) * vec4(aNormal,0.0));
    FragmentPosition = vec3(model * vec4(aPos,1.0));
    gl_Position = projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.0f);
}
