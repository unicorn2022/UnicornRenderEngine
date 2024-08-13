#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

/* 输入输出变量 */
in VS_OUT {
    vec3 Position;
    vec3 Normal;
} gs_in[];

const float MAGNITUDE = 0.01;

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};

void GenerateLine(int index_0, int index_1);

void main() {
    GenerateLine(0, 1);
    GenerateLine(1, 2);
    GenerateLine(2, 0);
}

void GenerateLine(int index_0, int index_1) {
    gl_Position = projection_transform * (gl_in[index_0].gl_Position + vec4(normalize(gs_in[index_0].Normal), 0.0) * MAGNITUDE); 
    EmitVertex();
    gl_Position = projection_transform * (gl_in[index_1].gl_Position + vec4(normalize(gs_in[index_1].Normal), 0.0) * MAGNITUDE); 
    EmitVertex();
    EndPrimitive();
}