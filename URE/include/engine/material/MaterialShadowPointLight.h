#pragma once
#include "engine/material/Material.h"

class MaterialShadowPointLight : public Material {
public:
    /** 材质: 点光源阴影
     * \param light_position 光源位置
     */
    MaterialShadowPointLight(glm::vec3 light_position);

public:
    virtual void Use();

public:
    /* 变换信息 */
    glm::mat4 view_transforms[6];
    glm::mat4 projection_transforms[6];

private:
    /* 材质信息 */
    glm::vec3 light_position;
};