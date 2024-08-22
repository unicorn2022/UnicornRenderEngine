#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 model_transform;


void main() {
    gl_Position = model_transform * vec4(aPos, 1.0); // 世界坐标下的顶点位置
}