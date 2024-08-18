#include "engine/material/MaterialShadowDirectLight.h"

MaterialShadowDirectLight::MaterialShadowDirectLight() {
    this->shader = new Shader("shadow/shadow_direct_light");
}

void MaterialShadowDirectLight::Use() {
    shader->Use();
}