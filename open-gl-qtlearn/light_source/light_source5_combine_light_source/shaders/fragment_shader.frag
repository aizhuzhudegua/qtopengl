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

// 聚光光源
struct SPotLight {
    // 光的位置
    vec3 position;
    // 聚光的方向
    vec3 spotDirection;
    // 切光角
    float cutoffAngle;
    // 聚光光滑处理的外圆切光
    float outerCutoffAngle;

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

// 定向光
struct DirectionLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 点光源
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 聚光光源
uniform SPotLight spotlight;
// 定向光
uniform DirectionLight directionlight;
// 点光源
uniform PointLight pointlights[4];


// 观察者位置(摄像机位置)
uniform vec3 viewPosition;

vec3 diffuseAndAmbientTextureColor = vec3(texture(material.diffuse,TextureCoords));
// 边框高光
vec3 specularTextureColor = vec3(texture(material.specular,TextureCoords));

vec3 calcSpotLight(vec3 normal,vec3 viewDirection);
vec3 calcDirectionLight(vec3 normal,vec3 viewDirection);
vec3 calcPointLight(PointLight light, vec3 normal,vec3 viewDirection);

void main()
{

    // 标准化法向量
    vec3 normal = normalize(Normal);
    // 计算观察方向向量
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);

    vec3 result=vec3(0);
    result += calcSpotLight(normal,viewDirection);
    result += calcDirectionLight(normal,viewDirection);
    for(int index = 0;index < 4;index++)
    {
        result += calcPointLight(pointlights[index],normal,viewDirection);
    }
    FragColor = vec4(result, 1.0);
}

// 计算点光源
vec3 calcSpotLight(vec3 normal,vec3 viewDirection)
{
    // 计算光照的方向(从聚光指向片段位置的方向)
    vec3 lightDir=normalize(FragmentPosition - spotlight.position);
    // 计算光照方向与聚光方向的夹角
    float theta = dot(lightDir, normalize(spotlight.spotDirection));
    // 内切光角与外切光角之差
    float epsilon = spotlight.cutoffAngle - spotlight.outerCutoffAngle;
    // 当前片段的聚光强度
    float intensity = clamp((theta - spotlight.outerCutoffAngle) / epsilon, 0.0, 1.0);


    // 计算光源到片段的距离
    float distance = length(spotlight.position - FragmentPosition);
    // 计算衰减后的强度
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance +
                               spotlight.quadratic * (distance * distance));

    // 计算环境光
    vec3 ambient = spotlight.ambient * diffuseAndAmbientTextureColor;
    /*
    * 漫反射计算所需的内容
    *   1. 法向量
    *   2. 定向的光线：光源位置与片段位置向量之差的方向
    * 定向光线的计算又需要如下内容：
    *   1. 光源的位置
    *   2. 片段的位置
    */
    // 标准化法向量
    // vec3 normal = normalize(Normal);
    // 光线方向(指的是从片段位置到光源的方向)
    vec3 lightDirection = normalize(spotlight.position - FragmentPosition);
    /*
      计算漫反射的结果
      max()是避免出现负值
    */
    vec3 diffuse = max(dot(normal,lightDirection),0.0) * diffuseAndAmbientTextureColor * spotlight.diffuse;

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
    //vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    // 计算反射光照向量，这里将lightDirection取反是因为reflect()的第一个参数是从光源指向片段的
    vec3 reflectDirection = reflect(-lightDirection,normal);
    // 计算镜面光照结果
    float middleSpecular = pow(max(dot(viewDirection,reflectDirection),0.0),material.shininess);
    vec3 specular = middleSpecular * spotlight.specular * specularTextureColor;

    return vec3(ambient + diffuse * attenuation * intensity + specular * attenuation * intensity);

}

// 计算定向光
vec3 calcDirectionLight(vec3 normal,vec3 viewDirection)
{
    vec3 lightDir = normalize(-directionlight.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = directionlight.ambient * diffuseAndAmbientTextureColor;
    vec3 diffuse = directionlight.diffuse * diff * diffuseAndAmbientTextureColor;
    vec3 specular = directionlight.specular * spec * specularTextureColor;
    return (ambient + diffuse + specular);
}

// 计算点光源
vec3 calcPointLight(PointLight light, vec3 normal,vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - FragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - FragmentPosition);
    float attenuation = 	1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TextureCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TextureCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
