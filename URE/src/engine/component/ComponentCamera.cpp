#include "engine/component/ComponentCamera.h"
#include "engine/gameobject/GO.h"
#include "engine/basic/UniformBuffer.h"

ComponentCamera::ComponentCamera(GO* gameobject, float fov, float near, float far, int width, int height) : Component(gameobject) {
    this->type = "component_camera";
    this->camera = new RoamingCameraPerspective((float)width / (float)height, fov, near, far);
    this->frame_buffer = new FrameBuffer(width, height);
}

ComponentCamera::~ComponentCamera() {
    delete frame_buffer;
    delete camera;
}

void ComponentCamera::UpdateCameraState() {
    ComponentTransform* transform = gameobject->GetComponent<ComponentTransform>();
    camera->SetPosition(transform->GetPosition());
    camera->SetYaw(transform->GetYaw());
    camera->SetPitch(transform->GetPitch());
    camera->UpdateCameraVector();
}

void ComponentCamera::ProcessMouseScroll(float yoffset) {
    float fov = camera->GetFOV();
    fov -= yoffset;
    camera->SetFOV(fov);
}

void ComponentCamera::RenderTick(std::vector<ComponentMesh*> &render_objects, std::vector<ComponentBorder*> &border_objects, ComponentMesh* skybox) {
    if (!enable) return;
    /* 1. 预处理 */
    {
        // 1.1 绑定帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->ID);
        // 1.2 修改视口大小
        glViewport(0, 0, frame_buffer->width, frame_buffer->height);
        // 1.3 更新 UniformBufferCamera 的值
        UniformBufferCamera::GetInstance().view_transform = camera->GetViewMatrix();
        UniformBufferCamera::GetInstance().projection_transform = camera->GetProjectionMatrix();
        UniformBufferCamera::GetInstance().UpdateUniformData();
        // 1.4 更新 UniformBufferLight 的值
        UniformBufferLight::GetInstance().UpdateUniformData();
    }
        
    /* 2. 清屏: 颜色缓冲, 深度缓冲, 模板缓冲 */
    {
        glClearColor(color_background.x, color_background.y, color_background.z, color_background.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    
    /* 3.1 绘制物体(1): 更新模板缓冲 */
    {
        glStencilFunc(GL_ALWAYS, 1, 0xff);  // 始终通过测试
        glStencilMask(0xff); // 写入的模板值不变(即为1)
        for (auto object : render_objects)
            object->Draw();
    }
    
    /* 3.2 绘制物体(2): 根据模板缓冲绘制边界 */
    if (GlobalValue::GetInstance().GetIntValue("show_border")) {
        glStencilFunc(GL_NOTEQUAL, 1, 0xff);// 模板值不为1时, 通过测试
        for (auto border : border_objects)
            border->Draw();
    }

    /* 4. 绘制天空盒 */
    if (skybox != NULL) {
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        skybox->Draw();
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
    }
    /* 5. 解除绑定帧缓冲 */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
