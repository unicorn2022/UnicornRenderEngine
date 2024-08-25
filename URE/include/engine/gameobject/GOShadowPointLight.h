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
     */
    GOShadowPointLight(std::string name, PointLight* point_light, int* shadow_map_index, int width, int height);
};
