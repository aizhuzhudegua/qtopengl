#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 Position;
uniform vec3 viewPos;
uniform samplerCube skybox;
void main() {
    // 计算光在两个不同物质之间传播的比值
    float ratio = 1.00 / 1.52;
    // 计算摄像头看向点的方向向量 I
    vec3 I = normalize(Position - viewPos);
    // 计算折射向量 R
    vec3 R = refract(I, normalize(Normal),ratio);
    // 根据折射向量进行纹理采样
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
