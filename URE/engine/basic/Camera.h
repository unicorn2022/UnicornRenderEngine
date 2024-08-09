#pragma once
#include "Utils.h"

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
    /* 漫游相机 */
    RoamingCamera() {}
    ~RoamingCamera(){}

public:
    void SetPosition(glm::vec3 position) { this->position = position; }
    void SetYaw(float yaw) { this->yaw = yaw; }
    void SetPitch(float pitch) { this->pitch = pitch; }

private:
    float yaw, pitch;   // 欧拉角

public:
    void UpdateCameraVector() {
        glm::vec3 front, right, up;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));

        /* 更新相机属性 */
        this->front = front;
        this->up = up;
    }
};

class RoamingCameraPerspective : public RoamingCamera {
public:
    /** 透视投影相机
     * \param aspect 宽高比
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     */
    RoamingCameraPerspective(float aspect, float fov, float near, float far) : RoamingCamera() {
        this->aspect = aspect;
        this->fov = fov;
        this->near = near;
        this->far = far;
        this->projection_transform = glm::perspective(glm::radians(fov), aspect, near, far);
    }

public:
    float GetFOV() const { return fov; }
    void SetFOV(float fov) {
        if (fov > 90.0f) fov = 90.0f;
        if (fov < 1.0f)  fov = 1.0f;
        this->fov = fov;
        projection_transform = glm::perspective(glm::radians(fov), aspect, near, far);
    }

private:
    float aspect; // 宽高比
    float fov;    // 视野
    float near;   // 近平面距离
    float far;    // 远平面距离
};