#include "engine/material/forward_rendering/MaterialRenderPhongModel.h"
#include "GlobalValue.h"
#include "engine/basic/UniformBuffer.h"

MaterialRenderPhongModel::MaterialRenderPhongModel(Texture* diffuse, Texture* specular, Texture* normal, float shininess) : Material("MaterialRenderPhongModel") {
    this->shader = new Shader("forward_rendering/mesh_render_phong_model");
    this->diffuse = diffuse;
    this->specular = specular;
    this->normal = normal;
    this->shininess = shininess;
}

MaterialRenderPhongModel::~MaterialRenderPhongModel() {
    delete diffuse;
    delete specular;
    delete normal;
}

void MaterialRenderPhongModel::Use() {
    shader->Use();
    /* 材质信息 */
    // 1. diffuse 贴图
    if (diffuse != NULL) {
        diffuse->Use(0);
        shader->SetUniform("material.diffuse", 0);
    }
    // 2. specular 贴图
    if (specular != NULL) {
        specular->Use(1);
        shader->SetUniform("material.specular", 1);
        shader->SetUniform("use_specular_map", 1);
    } else {
        shader->SetUniform("use_specular_map", 0);
    }
    // 3. normal 贴图
    if (normal != NULL) {
        normal->Use(2);
        shader->SetUniform("material.normal", 2);
        shader->SetUniform("use_normal_map", 1);
    } else {
        shader->SetUniform("use_normal_map", 0);
    }
    // 4. shiness
    shader->SetUniform("material.shininess", shininess);
    
    /* 光照模型信息 */
    use_blinn_phong = GlobalValue::GetInstance().GetIntValue("use_blinn_phong");
    shader->SetUniform("use_blinn_phong", use_blinn_phong);
    show_render_shadow = GlobalValue::GetInstance().GetIntValue("show_render_shadow");
    shader->SetUniform("show_render_shadow", show_render_shadow);

    /* 阴影贴图 */
    // 1. direct_light
    for (int i = 0; i < UniformBufferLight::GetInstance().use_direct_light_num; i++) {
        shader->SetUniform("direct_light_shadow_map_" + std::to_string(i), UniformBufferShadow::GetInstance().direct_light_shadow_map_index[i]);
    }
    // 2. point_light
    for (int i = 0; i < UniformBufferLight::GetInstance().use_point_light_num; i++) {
        shader->SetUniform("point_light_shadow_map_" + std::to_string(i), UniformBufferShadow::GetInstance().point_light_shadow_map_index[i]);
    }
    int last_point_index = std::max(UniformBufferLight::GetInstance().use_point_light_num - 1, 0);
    for (int i = UniformBufferLight::GetInstance().use_point_light_num; i < MAX_POINT_LIGHT_COUNT; i++) {
        shader->SetUniform("point_light_shadow_map_" + std::to_string(i), UniformBufferShadow::GetInstance().point_light_shadow_map_index[last_point_index]);
    }
}