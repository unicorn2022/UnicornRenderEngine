#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

/* 输入输出变量 */
out VS_OUT {
    vec3 ViewDirection;
} vs_out;

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};

void main() {
    vs_out.ViewDirection = aPos - vec3(0, 0, 0);
    
    mat4 view = mat4(mat3(view_transform));
    vec4 pos = projection_transform * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}