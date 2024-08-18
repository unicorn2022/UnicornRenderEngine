#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentMesh.h"
#include "engine/component/ComponentBorder.h"
#include "engine/material/Material.h"
#include "engine/basic/Camera.h"
#include "engine/basic/FrameBuffer.h"
#include "engine/basic/Light.h"

class ComponentShadow : public Component {
public:
    /** 阴影组件: GO可以产生阴影
     * \param gameobject 绑定的游戏对象
     * \param width 相机帧缓冲宽度
     * \param height 相机帧缓冲高度
     * \param samples 采样数
     */
    ComponentShadow(GO* gameobject, int width, int height, int samples);

    virtual ~ComponentShadow();

public:
    void UpdateCameraState();

public:
    /* 渲染逻辑 */
    void RenderTick(std::vector<ComponentMesh*> &render_objects);

public:
    RoamingCamera* camera;
    FrameBuffer* frame_buffer;
    Material* material;
};

class ComponentShadowDirectLight : public ComponentShadow {
public:
    /** 阴影组件: GO可以产生阴影
     * \param gameobject 绑定的游戏对象
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
    ComponentShadowDirectLight(GO* gameobject, DirectLight* direct_light, int width, int height, int samples, float near = 1.0f, float far = 7.5f, float left = -10.0f, float right = 10.0f, float bottom = -10.0f, float top = 10.0f);

private:
    DirectLight* direct_light;
};