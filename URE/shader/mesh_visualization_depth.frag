#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 输入输出变量 */
out vec4 FragColor;

/* uniform 变量 */
uniform float z_near;
uniform float z_far;

void main() {   
    float z = gl_FragCoord.z * 2.0 - 1.0; // [0, 1] => [-1, 1]
    float depth = (2.0 * z_near * z_far) / (z_far + z_near - z * (z_far - z_near)); // 计算深度
    vec3 color = vec3(depth / z_far);
    
    FragColor = vec4(color, 1.0);
}