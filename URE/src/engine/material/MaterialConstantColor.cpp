#include "engine/material/MaterialConstantColor.h"

MaterialConstantColor::MaterialConstantColor(glm::vec4 color) {
    this->shader = new Shader("mesh_constant_color");
    this->color = color;
}

void MaterialConstantColor::Use() {
    shader->Use();
    shader->SetUniform("model_transform", model_transform);
    shader->SetUniform("view_transform", view_transform);
    shader->SetUniform("projection_transform", projection_transform);
    shader->SetUniform("color", color);
}