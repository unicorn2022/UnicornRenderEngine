#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 model_transform;  // 实例化绘制

#define MAX_DIRECT_LIGHT_COUNT 2
#define MAX_POINT_LIGHT_COUNT 4
#define MAX_SPOT_LIGHT_COUNT 2

/* 输入输出变量 */
out VS_OUT {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 ViewPosition;
    mat3 TBN;
    vec4 direct_light_position[MAX_DIRECT_LIGHT_COUNT]; // 定向光源视角的坐标
    vec3 debug_color;
} vs_out;

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};
layout (std140, binding = 2) uniform ShadowMatrix {
    mat4 direct_light_matrix[MAX_DIRECT_LIGHT_COUNT];
    int use_direct_light_num_vert;
};

void main() {
    /* 计算输出 */
    vs_out.Position = vec3(model_transform * vec4(aPos, 1.0)); // 世界坐标下的顶点位置
    vs_out.Normal = mat3(transpose(inverse(model_transform))) * aNormal; // 保证法向量垂直平面
    vs_out.TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    vs_out.ViewPosition = view_position;

    /* 计算TBN矩阵 */
    vec3 T = normalize(vec3(model_transform * vec4(aTangent,    0.0)));
    vec3 B = normalize(vec3(model_transform * vec4(aBitangent,  0.0)));
    vec3 N = normalize(vec3(model_transform * vec4(aNormal,     0.0)));
    vs_out.TBN = mat3(T, B, N);

    /* 计算定向光照对应的顶点坐标 */
    for(int i = 0; i < use_direct_light_num_vert; i++)
        vs_out.direct_light_position[i] = direct_light_matrix[i] * vec4(vs_out.Position, 1.0);
    gl_Position = projection_transform * view_transform * vec4(vs_out.Position, 1.0);
}