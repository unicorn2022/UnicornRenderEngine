#pragma once
#include "Utils.h"
#include "Engine/Basic/Texture.h"

class FrameBuffer {
public:
    /** 帧缓冲对象, 包含一个颜色附件, 一个深度模板附件
     * \param width 宽度
     * \param height 高度
     */ 
    FrameBuffer(int width, int height) {
        this->width = width;
        this->height = height;
        CreateFrameBuffer();
    }
    ~FrameBuffer() {
        glDeleteRenderbuffers(1, &RBO);
        glDeleteFramebuffers(1, &ID);
        delete color_texture;
    }

public:
    /* 帧缓冲ID */
    unsigned int ID;
    /* 帧缓冲的宽高 */
    int width, height;
    /* 颜色附件(纹理) */
    Texture* color_texture;
    /* 深度模板附件(渲染缓冲对象) ID */
    unsigned int RBO;

private:
    void CreateFrameBuffer() {
        /* 1. 生成帧缓冲 */
        glGenFramebuffers(1, &ID);
        glBindFramebuffer(GL_FRAMEBUFFER, ID);

        /* 2. 创建纹理附件, 并将其作为颜色附件附加到帧缓冲上 */
        color_texture = new Texture(width, height);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,         // 帧缓冲目标
            GL_COLOR_ATTACHMENT0,   // 附件类型
            GL_TEXTURE_2D,          // 附加的纹理类型
            color_texture->ID,      // 附加的纹理对象
            0                       // mipmap级别
        );

        /* 3. 创建渲染缓冲对象附件, 并将其作为深度和模板附件附加到帧缓冲上 */
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,                 // 帧缓冲目标
            GL_DEPTH_STENCIL_ATTACHMENT,    // 附件类型
            GL_RENDERBUFFER,                // 附加的渲染缓冲对象类型
            RBO                             // 附加的渲染缓冲对象
        );

        /* 4. 检查帧缓冲是否完整 */
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "[ERROR::FrameBuffer.h::CreateFrameBuffer()] 渲染缓冲对象不完整\n";
        
        /* 5. 解除绑定 */
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};