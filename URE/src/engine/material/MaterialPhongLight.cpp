#include "engine/material/MaterialPhongLight.h"

MaterialPhongLight::MaterialPhongLight(Texture* diffuse, Texture* specular, float shininess) {
    this->shader = new Shader("mesh_phong_light");
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
}

MaterialPhongLight::~MaterialPhongLight() {
    delete diffuse;
    delete specular;
}

void MaterialPhongLight::Use() {
    shader->Use();
    /* 材质信息 */
    if (diffuse != NULL) {
        diffuse->Use(0);
        shader->SetUniform("material.diffuse", 0);
    }
    if (specular != NULL) {
        specular->Use(1);
        shader->SetUniform("material.specular", 1);
    }
    shader->SetUniform("material.shininess", shininess);
}