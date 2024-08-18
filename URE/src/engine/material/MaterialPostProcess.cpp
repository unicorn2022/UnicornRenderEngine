#include "engine/material/MaterialPostProcess.h"
#include "GlobalValue.h"

MaterialPostProcess::MaterialPostProcess(Texture* screen_texture, TextureMultiSample* screen_texture_multisample) {
    this->shader = new Shader("post_process/post_process");
    this->screen_texture = screen_texture;
    this->screen_texture_multisample = screen_texture_multisample;
    this->choose_post_process = 0;
}

void MaterialPostProcess::Use() {
    shader->Use();
    if (screen_texture != NULL) {
        screen_texture->Use(0);
        shader->SetUniform("screen_texture", 0);
    }
    if (screen_texture_multisample != NULL) {
        screen_texture_multisample->Use(1);
        shader->SetUniform("screen_texture_multisample", 1);
    }
    shader->SetUniform("choose_post_process", choose_post_process);
    shader->SetUniform("samples", screen_texture_multisample->samples);
    
    this->gamma = GlobalValue::GetInstance().GetFloatValue("gamma");
    shader->SetUniform("gamma", gamma);
}
