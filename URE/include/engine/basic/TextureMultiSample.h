#pragma once
#include "Utils.h"
#include "GlobalValue.h"

class TextureMultiSample {
public:
    /** 多重采样纹理
     * \param width 纹理的宽度
     * \param height 纹理的高度
     * \param samples 采样数
    */
    TextureMultiSample(int width, int height, int samples);

    ~TextureMultiSample();

public:
    /** 使用当前纹理
     * \param index 将纹理绑定的编号
     */
    void Use(int index);

public:
    /* 纹理ID */
    unsigned int ID;
    /* 纹理的宽高, 采样数 */
    int width, height, samples;

private:
    void CreateTextureMultiSample();
};