#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentLight.h"
#include "engine/component/ComponentMesh.h"

class ComponentCamera : public Component {
public:
    /** 摄像机组件: GO拥有观察场景的功能
     * \param gameobject 绑定的游戏对象
     */
    ComponentCamera(GO* gameobject) : Component(gameobject) {
        this->type = "component_camera";
        ComponentTransform* transform = gameobject->GetComponent<ComponentTransform>();
        this->camera = new RoamingCameraPerspective(
            (float)window_width / (float)window_height,
            transform->GetPosition(),
            main_camera_up,
            -90.0f,                 // 偏航角
            0.0f,                   // 俯仰角
            45.0f,                  // 视野, 0~90°
            main_camera_znear,      // 近平面距离
            main_camera_zfar        // 远平面距离
        );
        this->frame_buffer = new FrameBuffer(window_width, window_height);
    }

    virtual ~ComponentCamera() {
        delete frame_buffer;
        delete camera;
    }

public:
    void HandleKeyBoard(GLFWwindow* window, float delta_time) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            camera->ProcessKeyboard(CameraMovement::UP, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            camera->ProcessKeyboard(CameraMovement::DOWN, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera->ProcessKeyboard(CameraMovement::LEFT, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera->ProcessKeyboard(CameraMovement::RIGHT, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera->ProcessKeyboard(CameraMovement::FORWARD, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera->ProcessKeyboard(CameraMovement::BACKWARD, delta_time);
        }
    }
    
    void HandleMouseMovement(float x_offset, float y_offset) {
        camera->ProcessMouseMovement(x_offset, y_offset);
    }
    
    void HandleMouseScroll(float y_offset) {
        camera->ProcessMouseScroll(y_offset);
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