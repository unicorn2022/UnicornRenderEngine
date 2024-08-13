#include "engine/material/MaterialVisualizationNormal.h"

MaterialVisualizationNormal::MaterialVisualizationNormal(glm::vec4 color) {
    this->shader = new Shader("mesh_visualization_normal", true);
    this->color = color;
}

void MaterialVisualizationNormal::Use() {
    shader->Use();
    shader->SetUniform("model_transform", model_transform);
    shader->SetUniform("color", color);
}