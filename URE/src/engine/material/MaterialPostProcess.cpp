#include "engine/material/MaterialPostProcess.h"

MaterialPostProcess::MaterialPostProcess(Texture* screen_texture) {
    this->shader = new Shader("post_process");
    this->screen_texture = screen_texture;
    this->choose_post_process = 0;
}

void MaterialPostProcess::Use() {
    shader->Use();
    if (screen_texture != NULL) {
        screen_texture->Use(0);
        shader->SetUniform("screen_texture", 0);
    }
    shader->SetUniform("choose_post_process", choose_post_process);
}
