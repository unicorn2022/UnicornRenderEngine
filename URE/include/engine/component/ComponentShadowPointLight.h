#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentMesh.h"
#include "engine/component/ComponentBorder.h"
#include "engine/material/Material.h"
#include "engine/basic/Camera.h"
#include "engine/basic/FrameBuffer.h"
#include "engine/basic/Light.h"

class ComponentShadowPointLight : public Component {
public:
    /** 点光源阴影组件: GO可以产生点光源阴影
     * \param gameobject 绑定的游戏对象
     * \param point_light 对应UniformBuffer中的点光源
     * \param shadow_map_index 对应UniformBuffer中的阴影贴图索引
     * \param width 相机帧缓冲宽度
     * \param height 相机帧缓冲高度
     * \param near 正交投影相机: 近平面
     * \param far 正交投影相机: 远平面
     */
    ComponentShadowPointLight(GO* gameobject, PointLight* point_light, int* shadow_map_index, int width, int height, float near = 0.1f, float far = 100.0f);
    ~ComponentShadowPointLight();

public:
    void RenderTick();
    void UpdateCameraState();

public:
    RoamingCamera* camera;
    FrameBufferCube* frame_buffer_cube;
    Material* material;
    int* shadow_map_index;

private:
    PointLight* point_light;
};