#version 330 core
#extension GL_ARB_shading_language_include : enable

/* Phong 材质*/
struct PhongMaterial {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

/* 输入输出变量 */
out vec4 FragColor;
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

/* uniform 变量 */
uniform PhongMaterial material;

void main() {
    vec3 color = texture(material.diffuse, TexCoord).rgb;
    float alpha = texture(material.diffuse, TexCoord).a;
    FragColor = vec4(color, alpha);
}