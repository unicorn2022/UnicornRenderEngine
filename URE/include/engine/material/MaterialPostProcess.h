#pragma once
#include "engine/material/Material.h"
#include "engine/basic/Texture.h"

class MaterialPostProcess : public Material {
public:
    /** 材质: 后处理
     * \param screen_texture 屏幕纹理(从camera组件的帧缓冲中获取)
     */
    MaterialPostProcess(Texture* screen_texture);

public:
    virtual void Use();

public:
    Texture* screen_texture;
    int choose_post_process;
};