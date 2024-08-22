#pragma once
#include "Utils.h"
#include "engine/basic/Texture.h"
#include "engine/basic/TextureCube.h"
#include "engine/basic/TextureMultiSample.h"

class FrameBuffer {
public:
    FrameBuffer(int width, int height);
    virtual ~FrameBuffer() {}

public:
    virtual void Use();
    virtual void Convert() = 0;

protected:
    /* 帧缓冲ID */
    unsigned int ID;

    /* 帧缓冲的宽高 */
    int width, height;
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
    virtual void Convert();

private:
    /* 用于转换的帧缓冲ID */
    unsigned int convertID;
    
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

class FrameBufferCube : public FrameBuffer {
public:
    /** 帧缓冲对象, 包含一个颜色附件(立方纹理)
     * \param width 宽度
     * \param height 高度
     */ 
    FrameBufferCube(int width, int height);
    virtual ~FrameBufferCube();

public:
    virtual void Convert() {}

public:
    /* 颜色附件(立方纹理) */
    TextureCube* color_texture;

private:
    void CreateFrameBufferCube();
};