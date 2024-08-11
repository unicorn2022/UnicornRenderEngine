#include "engine/material/MaterialNoLight.h"

MaterialNoLight::MaterialNoLight(Texture* diffuse) {
    this->shader = new Shader("mesh_no_light");
    this->diffuse = diffuse;
}

MaterialNoLight::~MaterialNoLight() {
    delete diffuse;
}

void MaterialNoLight::Use() {
    shader->Use();
    shader->SetUniform("model_transform", model_transform);
    shader->SetUniform("view_transform", view_transform);
    shader->SetUniform("projection_transform", projection_transform);

    if (diffuse != NULL) {
        diffuse->Use(0);
        shader->SetUniform("material.diffuse", 0);
    }
}