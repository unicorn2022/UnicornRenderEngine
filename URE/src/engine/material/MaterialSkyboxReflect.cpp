#include "engine/material/MaterialSkyboxReflect.h"

MaterialSkyboxReflect::MaterialSkyboxReflect(TextureCube* skybox) : Material("MaterialSkyboxReflect") {
    this->shader = new Shader("shading/mesh_skybox_reflect");
    this->skybox = skybox;
}

MaterialSkyboxReflect::~MaterialSkyboxReflect() {
    delete skybox;
}

void MaterialSkyboxReflect::Use() {
    shader->Use();

    if (skybox != NULL) {
        skybox->Use(0);
        shader->SetUniform("skybox", 0);
    }
}