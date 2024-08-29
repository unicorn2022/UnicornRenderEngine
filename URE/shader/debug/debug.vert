#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 model_transform;  // 实例化绘制

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};

/* inout变量 */
out vec3 local_position;

void main() {
    local_position = aPos;
    vec3 position = vec3(model_transform * vec4(aPos, 1.0)); // 世界坐标下的顶点位置

    gl_Position = projection_transform * view_transform * vec4(position, 1.0);
}