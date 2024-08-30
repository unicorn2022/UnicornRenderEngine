#include "engine/material/forward_rendering/MaterialRenderPhongModel.h"
#include "GlobalValue.h"
#include "engine/basic/UniformBuffer.h"

MaterialRenderPhongModel::MaterialRenderPhongModel(Texture* diffuse, Texture* specular, Texture* normal, Texture* depth, float shininess) : Material("MaterialRenderPhongModel") {
    this->shader = new Shader("forward_rendering/mesh_render_phong_model");
    this->diffuse = diffuse;
    this->specular = specular;
    this->normal = normal;
    this->depth = depth;
    this->shininess = shininess;
}

MaterialRenderPhongModel::~MaterialRenderPhongModel() {
    delete diffuse;
    delete specular;
    delete normal;
    delete depth;
}

void MaterialRenderPhongModel::Use() {
    shader->Use();
    /* 材质信息 */
    // 1. diffuse 贴图
    if (diffuse != NULL) {
        diffuse->Use(texture_index_diffuse);
        shader->SetUniform("material.diffuse", texture_index_diffuse);
    }
    // 2. specular 贴图
    if (specular != NULL) {
        specular->Use(texture_index_specular);
        shader->SetUniform("material.specular", texture_index_specular);
        shader->SetUniform("material.use_specular_map", 1);
    } else {
        shader->SetUniform("material.use_specular_map", 0);
    }
    // 3. normal 贴图
    if (normal != NULL) {
        normal->Use(texture_index_normal);
        shader->SetUniform("material.normal", texture_index_normal);
        shader->SetUniform("material.use_normal_map", 1);
    } else {
        shader->SetUniform("material.use_normal_map", 0);
    }
    // 4. depth 贴图
    if (depth != NULL) {
        depth->Use(texture_index_depth);
        shader->SetUniform("material.depth", texture_index_depth);
        shader->SetUniform("material.use_depth_map", 1);
    } else {
        shader->SetUniform("material.use_depth_map", 0);
    }
    // 5. 其他变量
    shader->SetUniform("material.shininess", shininess);
    shader->SetUniform("material.height_scale", GlobalValue::GetInstance().GetFloatValue("height_scale"));
    
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