#pragma once
#include "engine/material/Material.h"

class MaterialVisualizationNormal : public Material {
public:
    /** 材质: 法向量可视化
     * \param color 法向量颜色
     */
    MaterialVisualizationNormal(glm::vec4 color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

public:
    virtual void Use();

private:
    /* 材质信息 */
    glm::vec4 color;
};