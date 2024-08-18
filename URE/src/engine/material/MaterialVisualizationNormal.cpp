#include "engine/material/MaterialVisualizationNormal.h"

MaterialVisualizationNormal::MaterialVisualizationNormal(glm::vec4 color) {
    this->shader = new Shader("visualization/mesh_visualization_normal", true);
    this->color = color;
}

void MaterialVisualizationNormal::Use() {
    shader->Use();
    shader->SetUniform("color", color);
}