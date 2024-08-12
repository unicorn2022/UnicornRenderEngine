#pragma once
#include "engine/gameobject/GO.h"
#include "engine/basic/Texture.h"
#include "GlobalValue.h"

class GOCapture2D : public GO {
public:
    /** 场景捕获2D
     * \param name GO名称
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     * \param width 相机帧缓冲宽度
     * \param height 相机帧缓冲高度
     */
    GOCapture2D(std::string name, float fov, float near, float far, int width = window_width, int height = window_height);

    Texture* GetCaptureTexture() const;
};