#version 330 core

out vec4 FragColor;
float near = 0.1;
float far = 100.0;
/*
  gl_FragCoord.z 存储这非线性的depth(0.0-1.0)之间
  https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/01%20Depth%20testing/
  此方法为OpenGL官网提供的将非线性depth转换为线性深度的方式，这个矩阵的推导未做研究
*/
float LinearizeDepth(float depth) {
    float z_ndc = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z_ndc  * (far - near));
}

/*
  根据官网depth线性计算和非线性计算公式，推导出的非线性depth转换为线性深度的方式
*/
float CustomLinearizeDepth(float depth) {
    return (far*near)/(depth*near+far*(1-depth));
}

void main() {
    // 非线性深度演示
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    // 线性深度演示，
    // 计算线性depth值 官方版
    //float depth = (LinearizeDepth(gl_FragCoord.z)-near) / (far-near);
    // 计算线性depth值 自定义版
    //float depth = (CustomLinearizeDepth(gl_FragCoord.z)-near) / (far-near);
    //FragColor = vec4(vec3(depth), 1.0);

}
