#include "engine/material/forward_rendering/MaterialNoLight.h"

MaterialNoLight::MaterialNoLight(Texture* diffuse) : Material("MaterialNoLight") {
    this->shader = new Shader("forward_rendering/mesh_no_light");
    this->diffuse = diffuse;
}

MaterialNoLight::~MaterialNoLight() {
    delete diffuse;
}

void MaterialNoLight::Use() {
    shader->Use();

    if (diffuse != NULL) {
        diffuse->Use(0);
        shader->SetUniform("material.diffuse", 0);
    }
}