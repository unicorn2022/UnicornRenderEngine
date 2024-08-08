#pragma once
#include "engine/material/Material.h"
#include "engine/basic/Texture.h"
#include "engine/basic/Light.h"

const static int MAX_POINT_LIGHT_COUNT = 4;

class MaterialPhongLight : public Material {
public:
    /** 材质: 无光照
     * \param diffuse 漫反射贴图
     * \param specular 高光度贴图
     * \param shininess 高光项
     */
    MaterialPhongLight(Texture* diffuse, Texture* specular, float shininess = 32.0f) {
        this->shader = new Shader("mesh_phong_light");
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }

    ~MaterialPhongLight() {
        delete diffuse;
        delete specular;
    }

public:
    virtual void Use() {
        shader->Use();
        /* 变换信息 */
        shader->SetUniform("model_transform", model_transform);
        shader->SetUniform("view_transform", view_transform);
        shader->SetUniform("projection_transform", projection_transform);

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

        /* 光照信息 */
        // 定向光
        shader->SetUniform("direct_light.ambient", direct_light->ambient);
        shader->SetUniform("direct_light.diffuse", direct_light->diffuse);
        shader->SetUniform("direct_light.specular", direct_light->specular);
        shader->SetUniform("direct_light.direction", direct_light->direction);
        // 点光源
        for (int i = 0; i < MAX_POINT_LIGHT_COUNT; i++) {
            shader->SetUniform("point_lights[" + std::to_string(i) + "].ambient", point_lights[i]->ambient);
            shader->SetUniform("point_lights[" + std::to_string(i) + "].diffuse", point_lights[i]->diffuse);
            shader->SetUniform("point_lights[" + std::to_string(i) + "].specular", point_lights[i]->specular);
            shader->SetUniform("point_lights[" + std::to_string(i) + "].position", point_lights[i]->position);
            shader->SetUniform("point_lights[" + std::to_string(i) + "].constant", point_lights[i]->constant);
            shader->SetUniform("point_lights[" + std::to_string(i) + "].linear", point_lights[i]->linear);
            shader->SetUniform("point_lights[" + std::to_string(i) + "].quadratic", point_lights[i]->quadratic);
        }
        // 聚光源
        shader->SetUniform("spot_light.ambient", spot_light->ambient);
        shader->SetUniform("spot_light.diffuse", spot_light->diffuse);
        shader->SetUniform("spot_light.specular", spot_light->specular);
        shader->SetUniform("spot_light.position", spot_light->position);
        shader->SetUniform("spot_light.direction", spot_light->direction);
        shader->SetUniform("spot_light.inner_cut_off", glm::cos(glm::radians(spot_light->inner_cut_off)));
        shader->SetUniform("spot_light.outer_cut_off", glm::cos(glm::radians(spot_light->outer_cut_off)));
        shader->SetUniform("spot_light.constant", spot_light->constant);
        shader->SetUniform("spot_light.linear", spot_light->linear);
        shader->SetUniform("spot_light.quadratic", spot_light->quadratic);
    }

public:
    /* 变换信息 */
    glm::mat4 model_transform;
    glm::mat4 view_transform;
    glm::mat4 projection_transform;
    /* 光照信息 */
    DirectLight* direct_light;
    PointLight* point_lights[MAX_POINT_LIGHT_COUNT];
    SpotLight* spot_light;

private:
    /* 材质信息 */
    Texture* diffuse;
    Texture* specular;
    float shininess;
};