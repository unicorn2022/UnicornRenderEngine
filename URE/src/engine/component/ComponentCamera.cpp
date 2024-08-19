#include "engine/component/ComponentCamera.h"
#include "engine/gameobject/GO.h"
#include "engine/basic/UniformBuffer.h"
#include "GameWorld.h"
#include "GameComponent.h"

ComponentCamera::ComponentCamera(GO* gameobject, float fov, float near, float far, int width, int height, int samples) : Component(gameobject) {
    this->type = "component_camera";
    this->camera = new RoamingCameraPerspective((float)width / (float)height, fov, near, far);
    this->frame_buffer = new FrameBuffer(width, height, samples);
}

ComponentCamera::~ComponentCamera() {
    delete frame_buffer;
    delete camera;
}

void ComponentCamera::UpdateCameraState() {
    ComponentTransform* transform = gameobject->GetComponents<ComponentTransform>()[0];
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

void ComponentCamera::RenderTick() {
    if (!enable) return;
    std::vector<ComponentMesh*> render_objects = GameComponent::GetInstance().GetComponentMesh(this->camera, true);
    std::vector<ComponentBorder*> border_components = GameComponent::GetInstance().GetComponentBorder();
    std::vector<ComponentShadow*> shadow_components = GameComponent::GetInstance().GetComponentShadow();
    ComponentMesh* skybox = GameWorld::GetInstance().skybox != NULL ? GameWorld::GetInstance().skybox->GetComponents<ComponentMesh>()[0] : NULL;

    // 渲染 test_camera 所见景象时禁止 test_camera_screen
    if (this == GameWorld::GetInstance().test_camera) {
        GameWorld::GetInstance().test_camera_screen->enable = false;
    }

    /* 1. 预处理 */
    {
        // 1.1 绑定帧缓冲
        frame_buffer->Use();
        // 1.2 更新 UniformBufferCamera 的值
        UniformBufferCamera::GetInstance().view_transform = camera->GetViewMatrix();
        UniformBufferCamera::GetInstance().projection_transform = camera->GetProjectionMatrix();
        UniformBufferCamera::GetInstance().UpdateUniformData();
        // 1.3 更新 UniformBufferLight 的值
        UniformBufferLight::GetInstance().UpdateUniformData();
        // 1.4 更新 UniformBufferShadow 的值
        int texture_index = 2;
        for (auto shadow_component : shadow_components) {
            shadow_component->frame_buffer->color_texture->Use(texture_index);
            *(shadow_component->light_matrix) = shadow_component->camera->GetProjectionMatrix() * shadow_component->camera->GetViewMatrix();
            *(shadow_component->shadow_map_index) = texture_index;
            texture_index++;
        }
        UniformBufferShadow::GetInstance().UpdateUniformData();
    }
        
    /* 2. 清屏: 颜色缓冲, 深度缓冲, 模板缓冲 */
    {
        glClearColor(color_background.x, color_background.y, color_background.z, color_background.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    
    /* 3.1 绘制不透明物体 */
    {
        // 3.1.1 绘制不含边框的物体, 不更新模板缓冲
        {
            glStencilFunc(GL_ALWAYS, 1, 0xff);  // 始终通过测试
            glStencilMask(0x00); // 写入的模板值为0
            for (auto object : render_objects)
                if (object->IsTransport() == false && object->gameobject->GetComponents<ComponentBorder>().size() == 0)
                    object->Draw();
        }
        // 3.1.2 绘制含边框的物体, 更新模板缓冲
        {
            glStencilFunc(GL_ALWAYS, 1, 0xff);  // 始终通过测试
            glStencilMask(0xff); // 写入的模板值不变(即为1)
            for (auto object : render_objects)
                if (object->IsTransport() == false && object->gameobject->GetComponents<ComponentBorder>().size() != 0)
                    object->Draw();
        }
        // 3.1.3 根据模板缓冲绘制边界
        if (GlobalValue::GetInstance().GetIntValue("show_border")) {
            glStencilFunc(GL_NOTEQUAL, 1, 0xff);// 模板值不为1时, 通过测试
            for (auto border : border_components)
                border->Draw();
        }
    }
    /* 3.2 绘制透明物体 */
    {
        glStencilFunc(GL_ALWAYS, 1, 0xff);  // 始终通过测试
        for (auto object : render_objects)
            if (object->IsTransport() == true)
                object->Draw();
    }

    /* 4. 绘制天空盒 */
    if (skybox != NULL) {
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
        skybox->Draw();
        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
    }

    /* 5. 转换帧缓冲的颜色附件 */
    frame_buffer->Convert();

    // 渲染 test_camera 所见景象时禁止 test_camera_screen
    if (this == GameWorld::GetInstance().test_camera) {
        GameWorld::GetInstance().test_camera_screen->enable = true;
    }
}
