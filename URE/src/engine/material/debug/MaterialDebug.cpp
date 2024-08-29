#include "engine/material/debug/MaterialDebug.h"

MaterialDebug::MaterialDebug(glm::vec4 color) : Material("MaterialDebug") {
    this->shader = new Shader("debug/debug");
    this->color = color;
}

void MaterialDebug::Use() {
    shader->Use();
    shader->SetUniform("color", color);
}