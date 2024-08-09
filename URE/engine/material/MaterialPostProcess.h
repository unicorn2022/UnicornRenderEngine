#pragma once
#include "engine/material/Material.h"

class MaterialPostProcess : public Material {
public:
    /** 材质: 后处理
     * \param screen_texture 屏幕纹理(从camera组件的帧缓冲中获取)
     */
    MaterialPostProcess(Texture* screen_texture) {
        this->shader = new Shader("post_process");
        this->screen_texture = screen_texture;
        this->choose_post_process = 0;
    }

public:
    virtual void Use() {
        shader->Use();
        if (screen_texture != NULL) {
            screen_texture->Use(0);
            shader->SetUniform("screen_texture", 0);
        }
        shader->SetUniform("choose_post_process", choose_post_process);
    }

public:
    Texture* screen_texture;
    int choose_post_process;
};