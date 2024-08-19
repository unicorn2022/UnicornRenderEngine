#include "engine/material/MaterialSkyboxRefract.h"

MaterialSkyboxRefract::MaterialSkyboxRefract(TextureCube* skybox) : Material("MaterialSkyboxRefract") {
    this->shader = new Shader("shading/mesh_skybox_refract");
    this->skybox = skybox;
}

MaterialSkyboxRefract::~MaterialSkyboxRefract() {
    delete skybox;
}

void MaterialSkyboxRefract::Use() {
    shader->Use();

    if (skybox != NULL) {
        skybox->Use(0);
        shader->SetUniform("skybox", 0);
    }
}