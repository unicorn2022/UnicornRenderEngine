#include "engine/material/MaterialVisualizationDepth.h"

MaterialVisualizationDepth::MaterialVisualizationDepth(float z_near, float z_far) : Material("MaterialVisualizationDepth") {
    this->shader = new Shader("visualization/mesh_visualization_depth");
    this->z_near = z_near;
    this->z_far = z_far;
}

void MaterialVisualizationDepth::Use() {
    shader->Use();
    shader->SetUniform("z_near", z_near);
    shader->SetUniform("z_far", z_far);
}