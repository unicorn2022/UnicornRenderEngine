#include "engine/material/MaterialShadowPointLight.h"

MaterialShadowPointLight::MaterialShadowPointLight(glm::vec3 light_position) : Material("MaterialShadowPointLight") {
    this->shader = new Shader("shadow/shadow_point_light", true);
    this->light_position = light_position;
}

void MaterialShadowPointLight::Use() {
    shader->Use();
    for (int i = 0; i < 6; i++) {
        shader->SetUniform("view_transforms[" + std::to_string(i) + "]", view_transforms[i]);
        shader->SetUniform("projection_transforms[" + std::to_string(i) + "]", projection_transforms[i]);
    }
    shader->SetUniform("light_position", light_position);
}