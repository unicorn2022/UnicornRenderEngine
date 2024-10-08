#include "engine/component/ComponentCamera.h"
#include "engine/gameobject/GO.h"
#include "engine/basic/UniformBuffer.h"
#include "GameWorld.h"
#include "GameComponent.h"
#include "engine/material/ALL.h"

ComponentCamera::ComponentCamera(GO* gameobject, float fov, float near, float far, int width, int height, int samples) : Component(gameobject) {
    this->type = "component_camera";
    this->camera = new RoamingCameraPerspective((float)width / (float)height, fov, near, far);
    // this->camera = new RoamingCameraOrtho(-10, 10, -10, 10, 0.1, 100);
    this->frame_buffer_2D = new FrameBuffer2D(width, height, samples);
}

ComponentCamera::~ComponentCamera() {
    delete frame_buffer_2D;
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
    auto camera = dynamic_cast<RoamingCameraPerspective*>(this->camera);
    if (camera == NULL) return;
    float fov = camera->GetFOV();
    fov -= yoffset;
    camera->SetFOV(fov);
}

void ComponentCamera::RenderTick() {
    if (!enable) return;
    auto component_meshs = GameComponent::GetInstance().GetComponentMesh(this->camera, true);
    auto component_borders = GameComponent::GetInstance().GetComponentBorder();
    auto component_shadow_direct_lights = GameComponent::GetInstance().GetComponentShadowDirectLight();
    auto component_shadow_point_lights = GameComponent::GetInstance().GetComponentShadowPointLight();
    ComponentMesh* skybox = GameWorld::GetInstance().skybox != NULL ? GameWorld::GetInstance().skybox->GetComponents<ComponentMesh>()[0] : NULL;

    // 渲染 test_camera 所见景象时禁止 test_camera_screen
    if (this == GameWorld::GetInstance().test_camera) {
        GameWorld::GetInstance().test_camera_screen->enable = false;
    }

    /* 1. 预处理 */
    {
        // 1.1 绑定帧缓冲
        frame_buffer_2D->Use();
        // 1.2 更新 UniformBufferCamera 的值
        UniformBufferCamera::GetInstance().view_transform = camera->GetViewMatrix();
        UniformBufferCamera::GetInstance().projection_transform = camera->GetProjectionMatrix();
        UniformBufferCamera::GetInstance().UpdateUniformData();
        // 1.3 更新 UniformBufferLight 的值
        UniformBufferLight::GetInstance().UpdateUniformData();
        // 1.4 更新 UniformBufferShadow 的值
        int texture_index = 4;
        // 1.4.1 定向光源阴影
        int use_direct_light_num = UniformBufferLight::GetInstance().use_direct_light_num;
        for (int i = 0; i < use_direct_light_num; i++) {
            auto shadow_component = component_shadow_direct_lights[i];
            dynamic_cast<FrameBuffer2D*>(shadow_component->frame_buffer_2D)->color_texture->Use(texture_index);
            *(shadow_component->light_matrix) = shadow_component->camera->GetProjectionMatrix() * shadow_component->camera->GetViewMatrix();
            *(shadow_component->shadow_map_index) = texture_index;
            texture_index++;
        }
        // 1.4.2 点光源阴影
        int use_point_light_num = UniformBufferLight::GetInstance().use_point_light_num;
        for (int i = 0; i < use_point_light_num; i++) {
            auto shadow_component = component_shadow_point_lights[i];
            dynamic_cast<FrameBufferCube*>(shadow_component->frame_buffer_cube)->color_texture->Use(texture_index);
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
            for (auto object : component_meshs)
                if (object->IsTransport() == false && object->gameobject->GetComponents<ComponentBorder>().size() == 0)
                    object->Draw();
        }
        // 3.1.2 绘制含边框的物体, 更新模板缓冲
        {
            glStencilFunc(GL_ALWAYS, 1, 0xff);  // 始终通过测试
            glStencilMask(0xff); // 写入的模板值不变(即为1)
            for (auto object : component_meshs)
                if (object->IsTransport() == false && object->gameobject->GetComponents<ComponentBorder>().size() != 0)
                    object->Draw();
        }
        // 3.1.3 根据模板缓冲绘制边界
        if (GlobalValue::GetInstance().GetIntValue("show_border") == 1) {
            glStencilFunc(GL_NOTEQUAL, 1, 0xff);// 模板值不为1时, 通过测试
            for (auto border : component_borders)
                border->Draw();
        }
    }
    /* 3.2 绘制透明物体 */
    {
        glStencilFunc(GL_ALWAYS, 1, 0xff);  // 始终通过测试
        for (auto object : component_meshs)
            if (object->IsTransport() == true)
                object->Draw();
    }

    /* 4. 绘制天空盒 */
    if (skybox != NULL && GlobalValue::GetInstance().GetIntValue("show_skybox") == 1) {
        glDepthFunc(GL_LEQUAL);
        if (use_opengl_cull_face) glDisable(GL_CULL_FACE);
        skybox->Draw();
        if (use_opengl_cull_face) glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
    }

    /* 5. 转换帧缓冲的颜色附件 */
    frame_buffer_2D->Convert();

    // 渲染 test_camera 所见景象时禁止 test_camera_screen
    if (this == GameWorld::GetInstance().test_camera) {
        GameWorld::GetInstance().test_camera_screen->enable = true;
    }
}
