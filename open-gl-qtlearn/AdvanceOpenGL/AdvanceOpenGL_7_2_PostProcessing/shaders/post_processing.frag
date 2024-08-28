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

// 核 采样偏移
const float offset = 1.0 / 300.0;

void main()
{
    // 反相
    //FragColor = vec4(vec3(1.0 - texture(material.diffuse, TextureCoords)), 1.0);

    // 灰度
    /*
    FragColor = texture(material.diffuse, TextureCoords);
    float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);
    */

    // 加权 人眼会对绿色更加敏感一些，而对蓝色不那么敏感，所以为了获取物理上更精确的效果，需要使用加权的(Weighted)通道
    /*
    FragColor = texture(material.diffuse, TextureCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
    */

    // 核(Kernel)（或卷积矩阵(Convolution Matrix)）是一个类矩阵的数值数组，它的中心为当前的像素，它会用它的核值乘以周围的像素值，并将结果相加变成一个值
    vec2 offsets[9] = vec2[](
            vec2(-offset,  offset), // 左上
            vec2( 0.0f,    offset), // 正上
            vec2( offset,  offset), // 右上
            vec2(-offset,  0.0f),   // 左
            vec2( 0.0f,    0.0f),   // 中
            vec2( offset,  0.0f),   // 右
            vec2(-offset, -offset), // 左下
            vec2( 0.0f,   -offset), // 正下
            vec2( offset, -offset)  // 右下
            );
    // 锐化(Sharpen)
    //    float kernel[9] = float[](
    //            -1, -1, -1,
    //             -1,  9, -1,
    //             -1, -1, -1
    //             );

    // 模糊(Blur)
    //    float kernel[9] = float[](
    //        1.0 / 16, 2.0 / 16, 1.0 / 16,
    //        2.0 / 16, 4.0 / 16, 2.0 / 16,
    //        1.0 / 16, 2.0 / 16, 1.0 / 16
    //    );

    // 边缘检测(Edge-detection)
    float kernel[9] = float[](
            1, 1, 1,
            1, -8, 1,
            1, 1, 1
             );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(material.diffuse, TextureCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}

