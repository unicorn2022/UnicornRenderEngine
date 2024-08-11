#pragma once
#include "Utils.h"
#include "Engine/Basic/Texture.h"

class FrameBuffer {
public:
    /** 帧缓冲对象, 包含一个颜色附件, 一个深度模板附件
     * \param width 宽度
     * \param height 高度
     */ 
    FrameBuffer(int width, int height);
    ~FrameBuffer();
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
    void CreateFrameBuffer();
};