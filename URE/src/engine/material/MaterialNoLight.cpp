#include "engine/material/MaterialNoLight.h"

MaterialNoLight::MaterialNoLight(Texture* diffuse) {
    this->shader = new Shader("shading/mesh_no_light");
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