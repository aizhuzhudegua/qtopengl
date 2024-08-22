#version 450 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTexcord;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texSmall;

void main()
{
//    FragColor = mix(texture2D(texture0,ourTexcord) , texture2D(texture1,ourTexcord),0.5);
    FragColor = texture2D(texSmall,ourTexcord);
};
