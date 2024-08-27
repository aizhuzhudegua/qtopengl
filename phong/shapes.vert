#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCord;
layout (location = 2) in vec3 aNormal;

out vec3 ourColor;
out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{

    gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z,1.0f);
    TexCoords = vec2(1-aTexCord.s,aTexCord.t);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalMatrix *  aNormal;

};
