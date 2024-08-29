#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 model_transform;  // 实例化绘制

/* 输入输出变量 */
out VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 ViewPosition;
} vs_out;

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};

void main() {
    vs_out.Position = vec3(model_transform * vec4(aPos, 1.0)); // 世界坐标下的顶点位置
    vs_out.Normal = mat3(transpose(inverse(model_transform))) * aNormal; // 保证法向量垂直平面
    vs_out.TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    vs_out.ViewPosition = view_position;

    gl_Position = projection_transform * view_transform * vec4(vs_out.Position, 1.0);
}