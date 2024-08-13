#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
    vec3 ViewDirection;
} vs_out;

uniform mat4 view_transform;
uniform mat4 projection_transform;

void main() {
    vs_out.ViewDirection = aPos - vec3(0, 0, 0);
    mat4 view = mat4(mat3(view_transform));
    vec4 pos = projection_transform * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}