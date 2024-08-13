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
    if (skybox_texture != NULL) {
        skybox_texture->Use(0);
        shader->SetUniform("skybox_texture", 0);
    }
}

TextureCube* MaterialSkybox::GetSkyboxTexture() const {
    return skybox_texture;
}
