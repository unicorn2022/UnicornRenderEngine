#include "engine/basic/TextureMultiSample.h"

TextureMultiSample::TextureMultiSample(int width, int height, int samples) {
    this->width = width;
    this->height = height;
    this->samples = samples;
    CreateTextureMultiSample();
}

TextureMultiSample::~TextureMultiSample() {
    glDeleteTextures(1, &ID);
}

void TextureMultiSample::Use(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
}

void TextureMultiSample::CreateTextureMultiSample() {
    /* 1. 绑定纹理ID */
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
    /* 2. 如果已经有宽高信息, 则申请空间 */
    if (width != 0 && height != 0)
        glTexImage2DMultisample(
            GL_TEXTURE_2D_MULTISAMPLE,  // 纹理目标
            samples,                    // 采样数
            GL_RGBA,                    // 将纹理存储为何种形式
            width, height,              // 宽高
            GL_TRUE                     // 每个像素使用相同的采样点位置
        );
}