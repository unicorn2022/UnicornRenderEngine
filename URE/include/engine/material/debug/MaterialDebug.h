#pragma once
#include "engine/material/Material.h"

class MaterialDebug : public Material {
public:
    /** 材质: 固定颜色
     * \param color 物体颜色
     */
    MaterialDebug(glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

public:
    virtual void Use();

private:
    /* 材质信息 */
    glm::vec4 color;
};