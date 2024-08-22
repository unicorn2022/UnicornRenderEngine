#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentMesh.h"
#include "engine/component/ComponentBorder.h"
#include "engine/material/Material.h"
#include "engine/basic/Camera.h"
#include "engine/basic/FrameBuffer.h"
#include "engine/basic/Light.h"

class ComponentShadowDirectLight : public Component {
public:
    /** 阴影组件: GO可以产生阴影
     * \param gameobject 绑定的游戏对象
     * \param direct_light 对应UniformBuffer中的方向光
     * \param light_matrix 对应UniformBuffer中的光源变换矩阵
     * \param shadow_map_index 对应UniformBuffer中的阴影贴图索引
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
    ComponentShadowDirectLight(GO* gameobject, DirectLight* direct_light, glm::mat4* light_matrix, int* shadow_map_index, int width, int height, int samples, float near = 0.1f, float far = 100.0f, float left = -10.0f, float right = 10.0f, float bottom = -10.0f, float top = 10.0f);
    ~ComponentShadowDirectLight();

public:
    void RenderTick();
    void UpdateCameraState();

public:
    RoamingCamera* camera;
    FrameBuffer2D* frame_buffer_2D;
    Material* material;
    glm::mat4* light_matrix;
    int* shadow_map_index;

private:
    DirectLight* direct_light;
};