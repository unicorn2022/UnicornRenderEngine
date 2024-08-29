#include "engine/material/visualization/MaterialVisualizationNormal.h"

MaterialVisualizationNormal::MaterialVisualizationNormal(glm::vec4 color) : Material("MaterialVisualizationNormal") {
    this->shader = new Shader("visualization/visualization_normal", true);
    this->color = color;
}

void MaterialVisualizationNormal::Use() {
    shader->Use();
    shader->SetUniform("color", color);
}