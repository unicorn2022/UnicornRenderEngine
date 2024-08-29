#include "engine/material/forward_rendering/MaterialReflectTextureCube.h"

MaterialReflectTextureCube::MaterialReflectTextureCube(TextureCube* texture_cube) : Material("MaterialReflectTextureCube") {
    this->shader = new Shader("forward_rendering/mesh_reflect_texture_cube");
    this->texture_cube = texture_cube;
}

MaterialReflectTextureCube::~MaterialReflectTextureCube() {
    delete texture_cube;
}

void MaterialReflectTextureCube::Use() {
    shader->Use();

    if (texture_cube != NULL) {
        texture_cube->Use(0);
        shader->SetUniform("texture_cube", 0);
    }
}