#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentMesh.h"
#include "engine/component/ComponentBorder.h"
#include "engine/basic/Camera.h"
#include "engine/basic/FrameBuffer.h"

class ComponentCamera : public Component {
public:
    /** 摄像机组件: GO拥有观察场景的功能
     * \param gameobject 绑定的游戏对象
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     * \param width 相机帧缓冲宽度
     * \param height 相机帧缓冲高度
     * \param samples 采样数
     */
    ComponentCamera(GO* gameobject, float fov, float near, float far, int width, int height, int samples);

    virtual ~ComponentCamera();

public:
    void UpdateCameraState();

    void ProcessMouseScroll(float yoffset);

public:
    /* 渲染逻辑 */
    void RenderTick();

public:
    RoamingCamera* camera;
    FrameBuffer2D* frame_buffer;
};