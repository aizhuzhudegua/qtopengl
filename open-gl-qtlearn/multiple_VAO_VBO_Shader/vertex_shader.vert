#version 330 core
/*
  顶点着色器，接收变量一般使用 layout (location = index) in type variable_name;的形式指定 index 支持的最大值可以通过
    GLint vertexAttributeCount;
    // 返回一个值，即顶点着色器可访问的 4 分量通用顶点属性的最大数量。该值必须至少为16
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributeCount); 进行获取，最少支持16个
*/
layout (location = 0) in vec3 aPos;
/*
  Uniform 是另一种从CPU的应用向GPU中的着色器发送数据的方式
  uniform是全局的，可以被任意着色器程序在任意阶段访问
  如果声明了一个uniform变量但未使用，编译器会默认移除这个变量，导致最后编译处的版本中不包含这个uniform变量
*/
out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos,1.0f);
    vertexColor = vec4(1.0f,0.5f,0.2f,1.0f);
}
