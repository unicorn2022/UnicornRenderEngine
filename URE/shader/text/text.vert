#version 460 core
layout (location = 0) in vec2 aPos; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 aTexCoord;

/* 输入输出变量 */
out VS_OUT {
    vec3 TexCoords;
} vs_out;

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};

void main() {
    gl_Position = projection_transform * vec4(aPos, 0.0, 1.0);
    vs_out.TexCoords = aTexCoord;
}