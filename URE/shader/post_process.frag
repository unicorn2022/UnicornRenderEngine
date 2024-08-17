#version 460 core
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
uniform int choose_post_process;
uniform sampler2D screen_texture;
uniform sampler2DMS screen_texture_multisample;
uniform int samples;
uniform float gamma;

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
    vec4 color;
    // 1 - 反相
    if (choose_post_process == 1) {
        color = vec4(1.0 - sample_color[4], 1.0);
    }
    // 2 - 灰度
    else if (choose_post_process == 2) {
        float average = 0.2126 * sample_color[4].r + 0.7152 * sample_color[4].g + 0.0722 * sample_color[4].b;
        color = vec4(average, average, average, 1.0);
    } 
    // 3 - 锐化
    else if (choose_post_process == 3) {
        color = vec4(UseKernel(kernel_sharpen), 1.0);
    } 
    // 4 - 模糊
    else if (choose_post_process == 4) {
        color = vec4(UseKernel(kernel_blur), 1.0);
    }
    // 5 - 模糊
    else if (choose_post_process == 5) {
        color = vec4(UseKernel(kernel_edge_detection), 1.0);
    }
    // 其他 - 无效果
    else {
        color = vec4(sample_color[4], 1.0);
    }

    color.rgb = pow(color.rgb, vec3(1.0 / gamma));
    FragColor = color;
}

void SampleColor() {
    const float offset = 1.0 / 300.0;
    /* 9 个样本点 */
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

    /* 获取9个样本点的颜色 */
    ivec2 tex_size = textureSize(screen_texture_multisample);
    for (int i = 0; i < 9; i++) {
        /* 源图像 */
        // vec3 color = texture(screen_texture, fs_in.TexCoord + offsets[i]).rgb;
        
        /* MSAA: 颜色为所有采样点的平均值 */
        vec3 color = vec3(0.0f);
        for (int j = 0; j < samples; j++) {
            vec2 tex_coord = fs_in.TexCoord + offsets[i];
            color += texelFetch(screen_texture_multisample, ivec2(tex_coord * tex_size), j).rgb;
        }
        color = color / samples;

        /* 记录样本点颜色 */
        sample_color[i] = color;
    }
}

vec3 UseKernel(float kernel[9]) {
    vec3 color = vec3(0.0f);
    for (int i = 0; i < 9; i++)
        color += sample_color[i] * kernel[i];
    return color;
}