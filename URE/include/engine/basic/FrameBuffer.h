#pragma once
#include "Utils.h"
#include "engine/basic/Texture.h"
#include "engine/Basic/TextureMultiSample.h"

class FrameBuffer {
public:
    virtual ~FrameBuffer() {}

public:
    virtual void Use() = 0;
    virtual void Convert() = 0;
};

class FrameBuffer2D : public FrameBuffer {
public:
    /** 帧缓冲对象, 包含一个颜色附件, 一个深度模板附件
     * \param width 宽度
     * \param height 高度
     * \param samples 采样数
     */ 
    FrameBuffer2D(int width, int height, int samples);
    virtual ~FrameBuffer2D();

public:
    virtual void Use();
    virtual void Convert();

private:
    /* 帧缓冲ID */
    unsigned int ID;
    /* 用于转换的帧缓冲ID */
    unsigned int convertID;

    /* 帧缓冲的宽高 */
    int width, height;
    
    /* 深度模板附件(渲染缓冲对象) ID */
    unsigned int RBO;

public:
    /* 采样数 */
    int samples;
    /* 颜色附件(纹理) */
    Texture* color_texture;
    /* 颜色附件(多重采样纹理) */
    TextureMultiSample* color_texture_multisample;

private:
    void CreateFrameBuffer2D();
};