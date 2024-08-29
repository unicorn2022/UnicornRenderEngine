#include "engine/material/skybox/MaterialSkybox.h"

MaterialSkybox::MaterialSkybox(TextureCube* skybox_texture) : Material("MaterialSkybox") {
    this->shader = new Shader("skybox/skybox");
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

void MaterialSkybox::SetSkyboxTexture(TextureCube* texture_cube) {
    skybox_texture = texture_cube;
}