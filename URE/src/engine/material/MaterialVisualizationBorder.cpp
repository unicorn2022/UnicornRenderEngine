#include "engine/material/MaterialVisualizationBorder.h"

MaterialVisualizationBorder::MaterialVisualizationBorder(glm::vec4 color) {
    this->shader = new Shader("mesh_visualization_border", true);
    this->color = color;
}

void MaterialVisualizationBorder::Use() {
    shader->Use();
    shader->SetUniform("color", color);
}