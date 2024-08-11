#pragma once
#include "engine/material/Material.h"

class MaterialDepth : public Material {
public:
    /** 材质: 深度可视化
     * \param z_near 近平面距离
     * \param z_far 远平面距离
     */
    MaterialDepth(float z_near, float z_far);

public:
    virtual void Use();

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