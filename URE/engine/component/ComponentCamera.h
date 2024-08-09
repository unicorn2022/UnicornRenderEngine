#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentLight.h"
#include "engine/component/ComponentMesh.h"

class ComponentCamera : public Component {
public:
    /** 摄像机组件: GO拥有观察场景的功能
     * \param gameobject 绑定的游戏对象
     * \param aspect 宽高比
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     */
    ComponentCamera(GO* gameobject, float aspect, float fov, float near, float far) : Component(gameobject) {
        this->type = "component_camera";
        this->camera = new RoamingCameraPerspective(aspect, fov, near, far);
        this->frame_buffer = new FrameBuffer(window_width, window_height);
    }

    virtual ~ComponentCamera() {
        delete frame_buffer;
        delete camera;
    }

public:
    void UpdateCameraState() {
        ComponentTransform* transform = gameobject->GetComponent<ComponentTransform>();
        camera->SetPosition(transform->GetPosition());
        camera->SetYaw(transform->GetYaw());
        camera->SetPitch(transform->GetPitch());
        camera->UpdateCameraVector();
    }

    void ProcessMouseScroll(float yoffset) {
        float fov = camera->GetFOV();
        fov -= yoffset;
        camera->SetFOV(fov);
    }

public:
    /* 渲染逻辑 */
    void RenderTick(std::vector<ComponentMesh*> render_objects, std::vector<ComponentLight*> lights, ComponentMesh* skybox) {
        // 2.2.0 绑定帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->ID);
        // 2.2.1 清屏: 颜色缓冲, 深度缓冲, 模板缓冲
        glClearColor(color_background.x, color_background.y, color_background.z, color_background.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 2.2.2 绘制物体
        for (auto object : render_objects){
            object->RenderTick(camera, lights);
        }
        // 2.2.3 绘制天空盒
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        skybox->RenderTick(camera, lights);
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
        // 2.2.4 解除绑定帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

public:
    RoamingCameraPerspective* camera;
    FrameBuffer* frame_buffer;
};