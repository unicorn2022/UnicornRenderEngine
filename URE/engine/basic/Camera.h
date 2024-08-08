#pragma once
#include "Utils.h"

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera{
public:
    glm::mat4 GetViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }
    glm::mat4 GetProjectionMatrix() const {
        return projection_transform;
    }
    
public:
    glm::vec3 position; // 位置
    glm::vec3 front;    // 向前方向
    glm::vec3 up;       // 向上方向
    glm::mat4 projection_transform;
};

class RoamingCamera : public Camera{
public:
    /** 相机
     * \param position 相机位置
     * \param up 相机向上方向
     * \param yaw 偏航角
     * \param pitch 俯仰角
     */
    RoamingCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
        this->front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->camera_speed = 2.5f;

        this->position = position;
        this->world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        UpdateCameraVector();   
    }
    ~RoamingCamera(){}

public:
    /** 控制 RoamingCamera 移动
     * \note `WASDQE` 对应 `前后左右下上`
     */
    void ProcessKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = camera_speed * deltaTime;
        if (direction == FORWARD)
            position += glm::normalize(front) * velocity;
        if (direction == BACKWARD)
            position -= glm::normalize(front) * velocity;
        if (direction == LEFT)
            position -= glm::normalize(right) * velocity;
        if (direction == RIGHT)
            position += glm::normalize(right) * velocity;
        if (direction == UP)
            position += glm::normalize(up) * velocity;
        if (direction == DOWN)
            position -= glm::normalize(up) * velocity;  
    }

    void ProcessMouseMovement(float xoffset, float yoffset) {
        yaw += xoffset;
        pitch += yoffset;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        UpdateCameraVector();
    }

    virtual void ProcessMouseScroll(float yoffset) {}

public:
    float camera_speed; // 移动速度    

private:
    glm::vec3 right;    // 向右方向
    glm::vec3 world_up; // 世界上坐标

    float yaw, pitch;   // 欧拉角

    void UpdateCameraVector() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->up    = glm::normalize(glm::cross(this->right, this->front));
    }
};

class RoamingCameraPerspective : public RoamingCamera {
public:
    /** 透视投影相机
     * \param aspect 宽高比
     * \param position 相机位置
     * \param up 相机向上方向
     * \param yaw 偏航角
     * \param pitch 俯仰角
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     */
    RoamingCameraPerspective(float aspect, glm::vec3 position, glm::vec3 up, float yaw, float pitch, float fov, float near, float far) 
    : RoamingCamera(position, up, yaw, pitch) {
        this->aspect = aspect;
        this->fov = fov;
        this->near = near;
        this->far = far;
        this->projection_transform = glm::perspective(glm::radians(fov), aspect, near, far);
    }

public:
    void ProcessMouseScroll(float yoffset) {
        fov -= yoffset;
        if (fov > 90.0f) fov = 90.0f;
        if (fov < 1.0f)  fov = 1.0f;
        projection_transform = glm::perspective(glm::radians(fov), aspect, near, far);
    }

private:
    float aspect; // 宽高比
    float fov;    // 视野
    float near;   // 近平面距离
    float far;    // 远平面距离
};

class RoamingCameraOrtho : public RoamingCamera {
public:
    /** 正交投影相机
     * \param position 相机位置
     * \param up 相机向上方向
     * \param yaw 偏航角
     * \param pitch 俯仰角
     * \param width 宽
     * \param height 高
     * \param near 近平面距离
     * \param far 远平面距离
     */
    RoamingCameraOrtho(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f, float width = 800.0f, float height = 600.0f, float near = 0.1f, float far = 100.0f) 
    : RoamingCamera(position, up, yaw, pitch) {
        this->width = width;
        this->height = height;
        this->near = near;
        this->far = far;
        this->projection_transform = glm::ortho(0.0f, width, 0.0f, height, near, far);
    }

private:
    float width;    // 宽
    float height;   // 高
    float near;     // 近平面距离
    float far;      // 远平面距离
};

