#include "engine/material/MaterialSkyboxRefract.h"

MaterialSkyboxRefract::MaterialSkyboxRefract(TextureCube* skybox) {
    this->shader = new Shader("mesh_skybox_refract");
    this->skybox = skybox;
}

MaterialSkyboxRefract::~MaterialSkyboxRefract() {
    delete skybox;
}

void MaterialSkyboxRefract::Use() {
    shader->Use();
    shader->SetUniform("model_transform", model_transform);

    if (skybox != NULL) {
        skybox->Use(0);
        shader->SetUniform("skybox", 0);
    }
}