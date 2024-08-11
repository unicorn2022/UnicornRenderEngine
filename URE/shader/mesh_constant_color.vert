#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model_transform;
uniform mat4 view_transform;
uniform mat4 projection_transform;

void main() {
    Position = vec3(model_transform * vec4(aPos, 1.0)); // 世界坐标下的顶点位置
    Normal = mat3(transpose(inverse(model_transform))) * aNormal; // 保证法向量垂直平面
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);

    gl_Position = projection_transform * view_transform * vec4(Position, 1.0);
}