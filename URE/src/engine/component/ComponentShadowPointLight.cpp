#include "engine/component/ComponentShadowPointLight.h"
#include "engine/basic/UniformBuffer.h"
#include "engine/gameobject/GO.h"
#include "engine/material/ALL.h"
#include "GameWorld.h"
#include "GameComponent.h"

ComponentShadowPointLight::ComponentShadowPointLight(GO* gameobject, PointLight* point_light, int* shadow_map_index, int width, int height) : Component(gameobject) {
    this->type = "component_shadow_point_light";
    this->camera = new RoamingCameraPerspective((float)width/(float)height, 90, 0.1f, POINT_LIGHT_SHADOW_ZFAR);
    this->point_light = point_light;
    this->shadow_map_index = shadow_map_index;
    this->material = new MaterialShadowPointLight(point_light->position);
    this->frame_buffer_cube = new FrameBufferCube(width, height);
}

ComponentShadowPointLight::~ComponentShadowPointLight() {
    delete frame_buffer_cube;
    delete camera;
    delete material;
}

void ComponentShadowPointLight::UpdateCameraState() {
    ComponentTransform* transform = gameobject->GetComponents<ComponentTransform>()[0];
    auto position = transform->GetPosition();
    camera->SetPosition(position);
}

void ComponentShadowPointLight::RenderTick() {
    if (!enable) return;
    std::vector<ComponentMesh*> render_objects = GameComponent::GetInstance().GetComponentMesh(this->camera, false);

    const glm::vec3 front_dir[6] = {
        glm::vec3( 1.0,  0.0,  0.0),
        glm::vec3(-1.0,  0.0,  0.0),
        glm::vec3( 0.0,  1.0,  0.0),
        glm::vec3( 0.0, -1.0,  0.0),
        glm::vec3( 0.0,  0.0,  1.0),
        glm::vec3( 0.0,  0.0, -1.0),
    };
    const glm::vec3 up_dir[6] = {
        glm::vec3(0.0, -1.0,  0.0),
        glm::vec3(0.0, -1.0,  0.0),
        glm::vec3(0.0,  0.0,  1.0),
        glm::vec3(0.0,  0.0, -1.0),
        glm::vec3(0.0, -1.0,  0.0),
        glm::vec3(0.0, -1.0,  0.0),
    };

    for (int face = 0; face < 6; face++) {
        /* 1. 预处理 */
        // 1.1 绑定帧缓冲
        frame_buffer_cube->Use(face);
        // 1.2 更新 UniformBufferCamera 的值
        camera->front = front_dir[face];
        camera->up = up_dir[face];
        UniformBufferCamera::GetInstance().view_transform = camera->GetViewMatrix();
        UniformBufferCamera::GetInstance().projection_transform = camera->GetProjectionMatrix();
        UniformBufferCamera::GetInstance().UpdateUniformData();

        
        /* 2. 清屏: 颜色缓冲, 深度缓冲, 模板缓冲 */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        /* 3. 使用特定材质, 绘制所有不透明物体 */
        glStencilFunc(GL_ALWAYS, 1, 0xff);  // 始终通过测试
        glStencilMask(0x00); // 写入的模板值为0
        for (auto object : render_objects)
            if (object->IsTransport() == false && object->IsDebug() == false)
                object->Draw(material);
    }


}