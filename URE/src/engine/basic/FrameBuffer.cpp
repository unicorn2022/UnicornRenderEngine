#include "engine/basic/FrameBuffer.h"

/* FrameBuffer */
FrameBuffer::FrameBuffer(int width, int height) {
    this->width = width;
    this->height = height;
}
void FrameBuffer::Use() {
    // 1.1 绑定帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    // 1.2 修改视口大小
    glViewport(0, 0, width, height);
}

/* FrameBuffer2D */
FrameBuffer2D::FrameBuffer2D(int width, int height, int samples) : FrameBuffer(width, height) {
    this->samples = samples;
    CreateFrameBuffer2D();
}
FrameBuffer2D::~FrameBuffer2D() {
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &ID);
    glDeleteFramebuffers(1, &convertID);
    delete color_texture_multisample;
    delete color_texture;
}
void FrameBuffer2D::Convert() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, convertID);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer2D::CreateFrameBuffer2D() {
    /* 1. 生成帧缓冲 */
    {
        // 1.1 帧缓冲对象
        glGenFramebuffers(1, &ID);
        glBindFramebuffer(GL_FRAMEBUFFER, ID);

        // 1.2 创建纹理附件, 并将其作为颜色附件附加到帧缓冲
        color_texture_multisample = new TextureMultiSample(width, height, samples);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,                 // 帧缓冲目标
            GL_COLOR_ATTACHMENT0,           // 附件类型
            GL_TEXTURE_2D_MULTISAMPLE,      // 附加的纹理类型
            color_texture_multisample->ID,  // 附加的纹理对象
            0                               // mipmap级别
        );

        // 1.3 创建渲染缓冲对象附件, 并将其作为深度和模板附件附加到帧缓冲上
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,                 // 帧缓冲目标
            GL_DEPTH_STENCIL_ATTACHMENT,    // 附件类型
            GL_RENDERBUFFER,                // 附加的渲染缓冲对象类型
            RBO                             // 附加的渲染缓冲对象
        );

        // 1.4 检查帧缓冲是否完整
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "[ERROR::FrameBuffer.h::CreateFrameBuffer()] 渲染缓冲对象不完整\n";
    }

    /* 2. 生成用于转换的帧缓冲 */
    {
        // 2.1 帧缓冲对象
        glGenFramebuffers(1, &convertID);
        glBindFramebuffer(GL_FRAMEBUFFER, convertID);
        // 1.2 创建纹理附件, 并将其作为颜色附件附加到帧缓冲
        color_texture = new Texture(width, height);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,             // 帧缓冲目标
            GL_COLOR_ATTACHMENT0,       // 附件类型
            GL_TEXTURE_2D,              // 附加的纹理类型
            color_texture->ID,          // 附加的纹理对象
            0                           // mipmap级别
        );
        // 2.3 检查帧缓冲是否完整
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "[ERROR::FrameBuffer.h::CreateFrameBuffer()] 转换帧缓冲对象不完整\n";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* FrameBufferCube */
FrameBufferCube::FrameBufferCube(int width, int height) : FrameBuffer(width, height) {
    CreateFrameBufferCube();
}
FrameBufferCube::~FrameBufferCube() {
    glDeleteFramebuffers(1, &ID);
    delete color_texture;
}
void FrameBufferCube::CreateFrameBufferCube() {
    // 1.1 帧缓冲对象
    glGenFramebuffers(1, &ID);
    glBindFramebuffer(GL_FRAMEBUFFER, ID);

    // 1.2 创建纹理附件, 并将其作为颜色附件附加到帧缓冲
    color_texture = new TextureCube(width, height);
    glFramebufferTexture(
        GL_FRAMEBUFFER,                 // 帧缓冲目标
        GL_COLOR_ATTACHMENT0,           // 附件类型
        color_texture->ID,              // 附加的纹理对象
        0                               // mipmap级别
    );

    // 1.3 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "[ERROR::FrameBuffer.h::CreateFrameBuffer()] 渲染缓冲对象不完整\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}