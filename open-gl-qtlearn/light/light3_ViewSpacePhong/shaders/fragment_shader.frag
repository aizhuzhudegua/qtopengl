#version 330 core
out vec4 FragColor;

// 法线数据
in vec3 Normal;
// 片段位置
in vec3 FragmentPosition;
// 光源位置
in vec3 LightPosition;

// 物体颜色
uniform vec3 objectColor;
// 光源颜色
uniform vec3 lightColor;

void main()
{
    // 添加环境光的强度
    float ambientStrength = 0.1;
    // 计算环境光
    vec3 ambient = ambientStrength * lightColor;
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
    // 光线方向
    vec3 lightDirection = normalize(LightPosition - FragmentPosition);
    /*
      计算漫反射的结果
      max()是避免出现负值
    */
    vec3 diffuse = max(dot(normal,lightDirection),0.0) * lightColor;

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

    // 指定镜面光照强度
    float specularStrength  = 0.5;
    // 计算观察方向向量，由于观察空间下，观察者永远处于原点位置，所以这里就相当于 (0,0,0) - Position => -Position
    vec3 viewDirection = normalize(-FragmentPosition);
    // 计算反射光照向量，这里将lightDirection取反是因为reflect()的第一个参数是从光源指向片段的
    vec3 reflectDirection = reflect(-lightDirection,normal);
    // 计算镜面光照结果
    float middleSpecular = pow(max(dot(viewDirection,reflectDirection),0.0),32);
    vec3 specular = specularStrength * middleSpecular * lightColor;
    FragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);

}

