#version 330 core
out vec4 FragColor;
// 高洛德着色结果
in vec3 GouraudResult;
void main()
{
    FragColor = vec4(GouraudResult, 1.0);

}

