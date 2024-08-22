#version 450 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTexcord;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = mix(texture2D(texture0,ourTexcord) , texture2D(texture1,ourTexcord),0.5);
};
