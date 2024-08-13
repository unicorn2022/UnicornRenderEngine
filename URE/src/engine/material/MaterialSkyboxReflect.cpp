#include "engine/material/MaterialSkyboxReflect.h"

MaterialSkyboxReflect::MaterialSkyboxReflect(TextureCube* skybox) {
    this->shader = new Shader("mesh_skybox_reflect");
    this->skybox = skybox;
}

MaterialSkyboxReflect::~MaterialSkyboxReflect() {
    delete skybox;
}

void MaterialSkyboxReflect::Use() {
    shader->Use();
    shader->SetUniform("model_transform", model_transform);

    if (skybox != NULL) {
        skybox->Use(0);
        shader->SetUniform("skybox", 0);
    }
}