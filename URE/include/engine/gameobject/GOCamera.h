#pragma once
#include "engine/gameobject/GO.h"

class GOCamera : public GO {
public:
    /** 相机游戏对象
     * \param name GO名称
     * \param aspect 宽高比
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     */
    GOCamera(std::string name, float aspect, float fov, float near, float far);
};