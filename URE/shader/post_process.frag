#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoord;
} fs_in;


/** 后期处理效果
 * 0 : 无效果
 * 1 : 反相
 * 2 : 灰度
 * 3 : 锐化
 * 4 : 模糊
 * 5 : 边缘检测
*/
uniform int choose_post_process = 0;
uniform sampler2D screen_texture;

vec3 sample_color[9];

float kernel_sharpen[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);
float kernel_blur[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
float kernel_edge_detection[9] = float[](
    1,  1, 1,
    1, -8, 1,
    1,  1, 1
);

void SampleColor();
vec3 UseKernel(float kernel[9]);

void main() {
    SampleColor();

    // 1 - 反相
    if (choose_post_process == 1) {
        FragColor = vec4(1.0 - sample_color[4], 1.0);
    }
    // 2 - 灰度
    else if (choose_post_process == 2) {
        float average = 0.2126 * sample_color[4].r + 0.7152 * sample_color[4].g + 0.0722 * sample_color[4].b;
        FragColor = vec4(average, average, average, 1.0);
    } 
    // 3 - 锐化
    else if (choose_post_process == 3) {
        FragColor = vec4(UseKernel(kernel_sharpen), 1.0);
    } 
    // 4 - 模糊
    else if (choose_post_process == 4) {
        FragColor = vec4(UseKernel(kernel_blur), 1.0);
    }
    // 5 - 模糊
    else if (choose_post_process == 5) {
        FragColor = vec4(UseKernel(kernel_edge_detection), 1.0);
    }
    // 其他 - 无效果
    else {
        FragColor = vec4(sample_color[4], 1.0);
    }
}

void SampleColor() {
    const float offset = 1.0 / 300.0;  
    const vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );
    for (int i = 0; i < 9; i++)
        sample_color[i] = texture(screen_texture, fs_in.TexCoord + offsets[i]).rgb;
}

vec3 UseKernel(float kernel[9]) {
    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++)
        color += sample_color[i] * kernel[i];
    return color;
}