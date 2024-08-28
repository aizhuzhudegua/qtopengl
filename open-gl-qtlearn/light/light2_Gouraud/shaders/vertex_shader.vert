#version 330 core
layout (location = 0) in vec3 aPos;
// 法线数据
layout (location = 1) in vec3 aNormal;

// 高洛德着色结果
out vec3 GouraudResult;

// 投影矩阵，作用是对显示内容剪裁及投影处理
uniform mat4 projection;
// 观察矩阵，作用将世界坐标系转换为相机坐标系下
uniform mat4 view;
// model矩阵，作用是将局部坐标转换为世界坐标
uniform mat4 model;

// 物体颜色
uniform vec3 objectColor;
// 光源颜色
uniform vec3 lightColor;
// 光源位置
uniform vec3 lightPosition;
// 观察者位置(摄像机位置)
uniform vec3 viewPosition;

void main()
{
    // 转换为世界坐标，因为这里的计算是在世界坐标系中计算的
    // vec4(aNormal,0.0)的0.0是为了法线的位移属性，也可以写作mat3(model) * aNormal
    // transpose(inverse(model)) 是为了求非等比缩放时法线矩阵
    vec3 Normal = vec3(transpose(inverse(model)) * vec4(aNormal,0.0));
    vec3 FragmentPosition = vec3(model * vec4(aPos,1.0));

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
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
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
    // 计算观察方向向量
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    // 计算反射光照向量，这里将lightDirection取反是因为reflect()的第一个参数是从光源指向片段的
    vec3 reflectDirection = reflect(-lightDirection,normal);
    // 计算镜面光照结果
    float middleSpecular = pow(max(dot(viewDirection,reflectDirection),0.0),32);
    vec3 specular = specularStrength * middleSpecular * lightColor;
    GouraudResult = (ambient + diffuse + specular) * objectColor;


    gl_Position = projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.0f);
}
