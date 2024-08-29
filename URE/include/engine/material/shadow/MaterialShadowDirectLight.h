#pragma once
#include "engine/material/Material.h"

class MaterialShadowDirectLight : public Material {
public:
    /* 材质: 定向光源阴影 */
    MaterialShadowDirectLight();

public:
    virtual void Use();
};