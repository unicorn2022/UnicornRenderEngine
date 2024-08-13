#pragma once
#include "engine/material/Material.h"

class MaterialConstantColor : public Material {
public:
    /** 材质: 固定颜色
     * \param color 物体颜色
     */
    MaterialConstantColor(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

public:
    virtual void Use();

public:
    /* 变换信息 */
    glm::mat4 model_transform;

private:
    /* 材质信息 */
    glm::vec4 color;
};