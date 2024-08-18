#pragma once
#include "engine/material/Material.h"

class MaterialShadowDirectLight : public Material {
public:
    /** 材质: 固定颜色
     * \param color 物体颜色
     */
    MaterialShadowDirectLight();

public:
    virtual void Use();
};