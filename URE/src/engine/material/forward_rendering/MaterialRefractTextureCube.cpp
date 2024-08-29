#include "engine/material/forward_rendering/MaterialRefractTextureCube.h"

MaterialRefractTextureCube::MaterialRefractTextureCube(TextureCube* texture_cube) : Material("MaterialRefractTextureCube") {
    this->shader = new Shader("forward_rendering/mesh_refract_texture_cube");
    this->texture_cube = texture_cube;
}

MaterialRefractTextureCube::~MaterialRefractTextureCube() {
    delete texture_cube;
}

void MaterialRefractTextureCube::Use() {
    shader->Use();

    if (texture_cube != NULL) {
        texture_cube->Use(0);
        shader->SetUniform("texture_cube", 0);
    }
}