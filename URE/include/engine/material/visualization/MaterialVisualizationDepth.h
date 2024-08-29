#pragma once
#include "engine/material/Material.h"

class MaterialVisualizationDepth : public Material {
public:
    /** 材质: 深度可视化
     * \param z_near 近平面距离
     * \param z_far 远平面距离
     */
    MaterialVisualizationDepth(float z_near, float z_far);

public:
    virtual void Use();

private:
    /* 材质信息 */
    float z_near;
    float z_far;
};