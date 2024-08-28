#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 Position;
uniform vec3 viewPos;
uniform samplerCube skybox;
void main() {
	// 计算摄像头看向点的方向向量 I
    vec3 I = normalize(Position - viewPos);
	// 计算反射向量 R
    vec3 R = reflect(I, normalize(Normal));
	// 根据反射向量进行纹理采样
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
