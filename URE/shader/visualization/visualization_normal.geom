#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

/* 输入输出变量 */
in VS_OUT {
    vec3 Position;
    vec3 Normal;
} gs_in[];

const float MAGNITUDE = 0.1;

/* uniform 变量 */
layout (std140, binding = 0) uniform CameraMatrix {
    mat4 view_transform;
    mat4 projection_transform;
    vec3 view_position;
};

void GenerateLine(int index);

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}

void GenerateLine(int index) {
    gl_Position = projection_transform * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection_transform * (gl_in[index].gl_Position + vec4(normalize(gs_in[index].Normal), 0.0) * MAGNITUDE); 
    EmitVertex();
    EndPrimitive();
}
