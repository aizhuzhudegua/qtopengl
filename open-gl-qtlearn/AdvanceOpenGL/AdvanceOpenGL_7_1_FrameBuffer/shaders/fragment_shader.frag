#version 330 core
out vec4 FragColor;

// 法线数据
in vec3 Normal;
// 片段位置
in vec3 FragmentPosition;
// 纹理坐标
in vec2 TextureCoords;

// 材质(物体颜色)
struct Material {
    /*
      vec3 ambient;
      vec3 diffuse;
      将环境光与漫反射光设置为一样的值，且从漫反射贴图中获取
    */
    sampler2D diffuse;
    // vec3 specular; 从镜面光贴图中获取镜面光的值
    sampler2D specular;
    float shininess;
};
uniform Material material;

// 光源颜色
struct Light {
    // 光的方向
    vec3 direction;
    // 环境光(也可表示强度)
    vec3 ambient;
    // 漫反射光(也可表示强度)
    vec3 diffuse;
    // 镜面光(也可表示强度)
    vec3 specular;
};
uniform Light light;

// 观察者位置(摄像机位置)
uniform vec3 viewPosition;
void main()
{
    FragColor = texture(material.diffuse,TextureCoords);
}

