#pragma once
#include "engine/material/Material.h"

class MaterialDepth : public Material {
public:
    /** 材质: 深度可视化
     * \param z_near 近平面距离
     * \param z_far 远平面距离
     */
    MaterialDepth(float z_near, float z_far) {
        this->shader = new Shader("mesh_depth");
        this->z_near = z_near;
        this->z_far = z_far;
    }

public:
    virtual void Use() {
        shader->Use();
        shader->SetUniform("model_transform", model_transform);
        shader->SetUniform("view_transform", view_transform);
        shader->SetUniform("projection_transform", projection_transform);
        shader->SetUniform("z_near", z_near);
        shader->SetUniform("z_far", z_far);
    }

public:
    /* 变换信息 */
    glm::mat4 model_transform;
    glm::mat4 view_transform;
    glm::mat4 projection_transform;

private:
    /* 材质信息 */
    float z_near;
    float z_far;
};