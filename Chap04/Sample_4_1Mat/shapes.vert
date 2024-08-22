#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCord;
out vec3 ourColor;
out vec2 ourTexcord;
uniform mat4 rotateMat4;
void main()
{
    ourColor = aColor;
    gl_Position = rotateMat4 * vec4(aPos.x,aPos.y,aPos.z,1.0f);
    ourTexcord = vec2(1-aTexCord.s,aTexCord.t);
};
