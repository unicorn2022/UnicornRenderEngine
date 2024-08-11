#include "engine/material/MaterialSkybox.h"

MaterialSkybox::MaterialSkybox(TextureCube* skybox_texture) {
    this->shader = new Shader("skybox");
    this->skybox_texture = skybox_texture;
}

MaterialSkybox::~MaterialSkybox() {
    delete skybox_texture;
}

void MaterialSkybox::Use() {
    shader->Use();
    shader->SetUniform("view_transform", view_transform);
    shader->SetUniform("projection_transform", projection_transform);
    if (skybox_texture != NULL) {
        skybox_texture->Use(0);
        shader->SetUniform("skybox_texture", 0);
    }
}
