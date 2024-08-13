#version 460 core
#extension GL_ARB_shading_language_include : enable

/* 材质*/
struct Material {
    sampler2D diffuse;
};

/* 输入输出变量 */
out vec4 FragColor;
in VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 ViewPosition;
} fs_in;

/* uniform 变量 */
uniform Material material;

void main() {
    vec3 color = texture(material.diffuse, fs_in.TexCoord).rgb;
    float alpha = texture(material.diffuse, fs_in.TexCoord).a;

    FragColor = vec4(color, alpha);
}