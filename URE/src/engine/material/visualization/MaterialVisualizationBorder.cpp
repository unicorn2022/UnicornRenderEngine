#include "engine/material/visualization/MaterialVisualizationBorder.h"

MaterialVisualizationBorder::MaterialVisualizationBorder(glm::vec4 color) : Material("MaterialVisualizationBorder") {
    this->shader = new Shader("visualization/visualization_border", true);
    this->color = color;
}

void MaterialVisualizationBorder::Use() {
    shader->Use();
    shader->SetUniform("color", color);
}