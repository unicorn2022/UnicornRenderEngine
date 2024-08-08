#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 TexCoord;

uniform mat4 view_transform;
uniform mat4 projection_transform;

void main() {
    TexCoord = aPos;
    mat4 view = mat4(mat3(view_transform));
    vec4 pos = projection_transform * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}