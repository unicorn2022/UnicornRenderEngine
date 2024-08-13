#include "engine/material/MaterialDepth.h"

MaterialDepth::MaterialDepth(float z_near, float z_far) {
    this->shader = new Shader("mesh_depth");
    this->z_near = z_near;
    this->z_far = z_far;
}

void MaterialDepth::Use() {
    shader->Use();
    shader->SetUniform("model_transform", model_transform);
    shader->SetUniform("z_near", z_near);
    shader->SetUniform("z_far", z_far);
}