#include "engine/component/ComponentCamera.h"
#include "engine/gameobject/GO.h"

ComponentCamera::ComponentCamera(GO* gameobject, float aspect, float fov, float near, float far) : Component(gameobject) {
    this->type = "component_camera";
    this->camera = new RoamingCameraPerspective(aspect, fov, near, far);
    this->frame_buffer = new FrameBuffer(window_width, window_height);
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

void ComponentCamera::RenderTick(std::vector<ComponentMesh*> render_objects, std::vector<ComponentLight*> lights, ComponentMesh* skybox) {
    // 2.2.0 绑定帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->ID);
    // 2.2.1 清屏: 颜色缓冲, 深度缓冲, 模板缓冲
    glClearColor(color_background.x, color_background.y, color_background.z, color_background.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 2.2.2 绘制物体
    for (auto object : render_objects){
        object->Draw(camera, lights);
    }
    // 2.2.3 绘制天空盒
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    skybox->Draw(camera, lights);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    // 2.2.4 解除绑定帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
