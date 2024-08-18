#pragma once
#include "engine/gameobject/GO.h"
#include "GlobalValue.h"

class GOShadowDirectLight : public GO {
public:
    /** 定向光源阴影游戏对象
     * \param name GO名称
     * \param direct_light 对应的方向光
     * \param width 相机帧缓冲宽度
     * \param height 相机帧缓冲高度
     * \param samples 采样数
     * \param near 正交投影相机: 近平面
     * \param far 正交投影相机: 远平面
     * \param left 正交投影相机: 左边界
     * \param right 正交投影相机: 右边界
     * \param bottom 正交投影相机: 下边界
     * \param top 正交投影相机: 上边界
     */
    GOShadowDirectLight(std::string name, DirectLight* direct_light, int width, int height, int samples, float near = 0.1f, float far = 100.0f, float left = -10.0f, float right = 10.0f, float bottom = -10.0f, float top = 10.0f);
};
