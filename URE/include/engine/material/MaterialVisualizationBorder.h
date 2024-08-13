#pragma once
#include "engine/material/Material.h"

class MaterialVisualizationBorder : public Material {
public:
    /** 材质: 边框可视化
     * \param color 法向量颜色
     */
    MaterialVisualizationBorder(glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

public:
    virtual void Use();

private:
    /* 材质信息 */
    glm::vec4 color;
};