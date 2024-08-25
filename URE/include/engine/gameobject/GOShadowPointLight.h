#pragma once
#include "engine/gameobject/GO.h"
#include "GlobalValue.h"

class GOShadowPointLight : public GO {
public:
    /** 点光源阴影游戏对象
     * \param name GO名称
     * \param point_light 对应UniformBuffer中的点光源
     * \param shadow_map_index 对应UniformBuffer中的阴影贴图索引
     * \param width 相机帧缓冲宽度
     * \param height 相机帧缓冲高度
     * \param near 正交投影相机: 近平面
     * \param far 正交投影相机: 远平面
     */
    GOShadowPointLight(std::string name, PointLight* point_light, int* shadow_map_index, int width, int height, float near = 0.1f, float far = 100.0f);
};
