#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};
uniform mat4 model_transform;

void main() {
    vec3 position = vec3(model_transform * vec4(aPos, 1.0)); // 世界坐标下的顶点位置

    gl_Position = projection_transform * view_transform * vec4(position, 1.0);
}