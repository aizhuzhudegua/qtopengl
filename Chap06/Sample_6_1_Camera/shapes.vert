#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCord;

out vec3 ourColor;
out vec2 ourTexcord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z,1.0f);
    ourTexcord = vec2(1-aTexCord.s,aTexCord.t);
};
