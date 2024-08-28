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
    // 光的位置
    vec3 position;
    // 环境光(也可表示强度)
    vec3 ambient;
    // 漫反射光(也可表示强度)
    vec3 diffuse;
    // 镜面光(也可表示强度)
    vec3 specular;

    /*用来处理光线强度的衰减*/
    // 衰减的常数
    float constant;
    // 一次项的系数
    float linear;
    // 二次项的系数
    float quadratic;
};
uniform Light light;

// 观察者位置(摄像机位置)
uniform vec3 viewPosition;
void main()
{
    vec3 diffuseAndAmbientTextureColor = vec3(texture(material.diffuse,TextureCoords));
    // 边框高光
    vec3 specularTextureColor = vec3(texture(material.specular,TextureCoords));
    // 木头反光，边框不反光
    //vec3 specularTextureColor =vec3(1) - vec3(texture(material.specular,TextureCoords));

    // 计算光源到片段的距离
    float distance = length(light.position - FragmentPosition);
    // 计算衰减后的强度
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                                    light.quadratic * (distance * distance));

    // 计算环境光
    vec3 ambient = light.ambient * diffuseAndAmbientTextureColor;
    /*
    * 漫反射计算所需的内容
    *   1. 法向量
    *   2. 定向的光线：光源位置与片段位置向量之差的方向
    * 定向光线的计算又需要如下内容：
    *   1. 光源的位置
    *   2. 片段的位置
    */
    // 标准化法向量
    vec3 normal = normalize(Normal);
    // 光线方向(指的是从片段位置到光源的方向)
    vec3 lightDirection = normalize(light.position - FragmentPosition);
    /*
      计算漫反射的结果
      max()是避免出现负值
    */
    vec3 diffuse = max(dot(normal,lightDirection),0.0) * diffuseAndAmbientTextureColor * light.diffuse;

    /*
    * 镜面光照计算所需的内容与漫反射相比多了一个额外的观察向量：从相机位置指向片段位置的向量
    *   1. 指定镜面光照强度
    *   2. 计算观察方向向量
    *   3. 计算反射光照向量
    *   4. 计算镜面光照结果
    *       1. 计算观察方向向量与反射光照向量的夹角，并舍去负数
    *       2. 进行反光度处理
    *       3. 将处理结果与镜面光照前度与光源的颜色进行相乘
    */

    // 计算观察方向向量
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    // 计算反射光照向量，这里将lightDirection取反是因为reflect()的第一个参数是从光源指向片段的
    vec3 reflectDirection = reflect(-lightDirection,normal);
    // 计算镜面光照结果
    float middleSpecular = pow(max(dot(viewDirection,reflectDirection),0.0),material.shininess);
    vec3 specular = middleSpecular * light.specular * specularTextureColor;
    FragColor = vec4((ambient * attenuation + diffuse * attenuation + specular * attenuation), 1.0);

}

