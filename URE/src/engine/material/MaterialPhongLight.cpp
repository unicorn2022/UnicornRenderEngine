#include "engine/material/MaterialPhongLight.h"
#include "GlobalValue.h"
#include "engine/basic/UniformBuffer.h"

MaterialPhongLight::MaterialPhongLight(Texture* diffuse, Texture* specular, float shininess) {
    this->shader = new Shader("shading/mesh_phong_light");
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
    
    /* 光照模型信息 */
    use_blinn_phong = GlobalValue::GetInstance().GetIntValue("use_blinn_phong");
    shader->SetUniform("use_blinn_phong", use_blinn_phong);

    /* 阴影贴图 */
    // 1. direct_light
    for (int i = 0; i < UniformBufferLight::GetInstance().use_direct_light_num; i++) {
        shader->SetUniform("direct_light_shadow_map_" + std::to_string(i), UniformBufferShadow::GetInstance().direct_light_shadow_map_index[i]);
    }
    // 2. point_light
    for (int i = 0; i < UniformBufferLight::GetInstance().use_point_light_num; i++) {
        shader->SetUniform("point_light_shadow_map_" + std::to_string(i), UniformBufferShadow::GetInstance().point_light_shadow_map_index[i]);
    }
}