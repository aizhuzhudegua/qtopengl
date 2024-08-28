#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
in vec2 TextureCord;
// 关联第几个纹理单元，默认是0
uniform sampler2D texture;
void main()
{
    FragColor = texture2D(texture,TextureCord);

}

