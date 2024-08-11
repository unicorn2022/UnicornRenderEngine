#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentLight.h"
#include "engine/component/ComponentMesh.h"
#include "engine/basic/Camera.h"
#include "engine/basic/FrameBuffer.h"

class ComponentCamera : public Component {
public:
    /** 摄像机组件: GO拥有观察场景的功能
     * \param gameobject 绑定的游戏对象
     * \param aspect 宽高比
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     */
    ComponentCamera(GO* gameobject, float aspect, float fov, float near, float far);

    virtual ~ComponentCamera();

public:
    void UpdateCameraState();

    void ProcessMouseScroll(float yoffset);

public:
    /* 渲染逻辑 */
    void RenderTick(std::vector<ComponentMesh*> render_objects, std::vector<ComponentLight*> lights, ComponentMesh* skybox);

public:
    RoamingCameraPerspective* camera;
    FrameBuffer* frame_buffer;
};