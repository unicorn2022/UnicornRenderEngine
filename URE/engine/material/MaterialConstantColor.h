#pragma once
#include "engine/material/Material.h"

class MaterialConstantColor : public Material {
public:
    /** 材质: 固定颜色
     * \param color 物体颜色
     */
    MaterialConstantColor(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
        this->shader = new Shader("mesh_constant_color");
        this->color = color;
    }

public:
    virtual void Use() {
        shader->Use();
        shader->SetUniform("model_transform", model_transform);
        shader->SetUniform("view_transform", view_transform);
        shader->SetUniform("projection_transform", projection_transform);
        shader->SetUniform("color", color);
    }

public:
    /* 变换信息 */
    glm::mat4 model_transform;
    glm::mat4 view_transform;
    glm::mat4 projection_transform;

private:
    /* 材质信息 */
    glm::vec4 color;
};