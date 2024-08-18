#include "engine/material/MaterialConstantColor.h"

MaterialConstantColor::MaterialConstantColor(glm::vec4 color) {
    this->shader = new Shader("shading/mesh_constant_color");
    this->color = color;
}

void MaterialConstantColor::Use() {
    shader->Use();
    shader->SetUniform("color", color);
}