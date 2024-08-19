#include "engine/material/MaterialShadowDirectLight.h"

MaterialShadowDirectLight::MaterialShadowDirectLight() : Material("MaterialShadowDirectLight") {
    this->shader = new Shader("shadow/shadow_direct_light");
}

void MaterialShadowDirectLight::Use() {
    shader->Use();
}