#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
in vec2 TextureCord;
// 关联第几个纹理单元，默认是0
uniform sampler2D textureWall;
uniform sampler2D textureSmile;
void main()
{
    // 混合textureWall和textureSmile
    FragColor = mix(texture2D(textureWall,TextureCord),texture2D(textureSmile,TextureCord),0.1f);

}

