#include "engine/component/ComponentShadowDirectLight.h"
#include "engine/basic/UniformBuffer.h"
#include "engine/material/ALL.h"
#include "engine/gameobject/GO.h"
#include "GameWorld.h"
#include "GameComponent.h"

ComponentShadowDirectLight::ComponentShadowDirectLight(GO* gameobject, DirectLight* direct_light, glm::mat4* light_matrix, int* shadow_map_index, int width, int height, int samples, float near, float far, float left, float right, float bottom, float top) : Component(gameobject) {
    this->type = "component_shadow_direct_light";
    this->camera = new RoamingCameraOrtho(left, right, bottom, top, near, far);
    // this->camera = new RoamingCameraPerspective(1.0f, 45, 0.1f, 1000.0f);
    this->direct_light = direct_light;
    this->light_matrix = light_matrix;
    this->shadow_map_index = shadow_map_index;
    this->material = new MaterialShadowDirectLight();
    this->frame_buffer_2D = new FrameBuffer2D(width, height, samples);
}

ComponentShadowDirectLight::~ComponentShadowDirectLight() {
    delete frame_buffer_2D;
    delete camera;
    delete material;
}

void ComponentShadowDirectLight::UpdateCameraState() {
    ComponentTransform* transform = gameobject->GetComponents<ComponentTransform>()[0];
    auto direction = 10.0f * transform->GetPosition();
    camera->SetPosition(-direction);
    camera->front = direction; // 保证 look-at 的 target 为原点
    if (Utils::IsSameDirection(direction, glm::vec3(0, 1, 0))) camera->up = glm::vec3(0, 0, 1);
    else camera->up = glm::vec3(0, 1, 0);
}

void ComponentShadowDirectLight::RenderTick() {
    if (!enable) return;
    std::vector<ComponentMesh*> render_objects = GameComponent::GetInstance().GetComponentMesh(this->camera, false);

    /* 禁用模板测试 */
    glDisable(GL_STENCIL_TEST);
    /* 1. 预处理 */
    // 1.1 绑定帧缓冲
    frame_buffer_2D->Use();
    // 1.2 更新 UniformBufferCamera 的值
    UpdateCameraState();
    UniformBufferCamera::GetInstance().view_transform = camera->GetViewMatrix();
    UniformBufferCamera::GetInstance().projection_transform = camera->GetProjectionMatrix();
    UniformBufferCamera::GetInstance().UpdateUniformData();
        
    /* 2. 清屏: 颜色缓冲, 深度缓冲 */
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* 3. 使用特定材质, 绘制所有不透明物体 */
    for (auto object : render_objects)
        if (object->IsTransport() == false && object->IsDebug() == false)
            object->Draw(material);
    
    /* 4. 转换帧缓冲的颜色附件 */
    frame_buffer_2D->Convert();
    
    /* 重新启用模板测试 */
    glEnable(GL_STENCIL_TEST);
}