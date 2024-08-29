#include "engine/material/shadow/MaterialShadowPointLight.h"

MaterialShadowPointLight::MaterialShadowPointLight(glm::vec3 light_position) : Material("MaterialShadowPointLight") {
    this->shader = new Shader("shadow/shadow_point_light");
    this->light_position = light_position;
}

void MaterialShadowPointLight::Use() {
    shader->Use();
    shader->SetUniform("light_position", light_position);
}