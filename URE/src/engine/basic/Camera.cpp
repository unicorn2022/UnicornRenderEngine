#include "engine/basic/Camera.h"

/* Camera */
glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}
glm::mat4 Camera::GetProjectionMatrix() const {
    return projection_transform;
}

/* RoamingCamera */
RoamingCamera::RoamingCamera() {}
RoamingCamera::~RoamingCamera(){}
void RoamingCamera::SetPosition(glm::vec3 position) { 
    this->position = position; 
}
void RoamingCamera::SetYaw(float yaw) { 
    this->yaw = yaw; 
}
void RoamingCamera::SetPitch(float pitch) { 
    this->pitch = pitch; 
}
void RoamingCamera::UpdateCameraVector() {
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


/* RoamingCameraPerspective */
RoamingCameraPerspective::RoamingCameraPerspective(float aspect, float fov, float near, float far) : RoamingCamera() {
    this->aspect = aspect;
    this->near = near;
    this->far = far;
    SetFOV(fov);
}
float RoamingCameraPerspective::GetFOV() const { 
    return fov; 
}
void RoamingCameraPerspective::SetFOV(float fov) {
    if (fov > 90.0f) fov = 90.0f;
    if (fov < 1.0f)  fov = 1.0f;
    this->fov = fov;
    projection_transform = glm::perspective(glm::radians(fov), aspect, near, far);
}

/* RoamingCameraOrtho */
RoamingCameraOrtho::RoamingCameraOrtho(float left, float right, float bottom, float top, float near, float far) {
    this->near = near;
    this->far = far;
    SetOrthoSquare(left, right, bottom, top);
}
void RoamingCameraOrtho::SetOrthoSquare(float left, float right, float bottom, float top) {
    this->left = left;
    this->right = right;
    this->bottom = bottom;
    this->top = top;
    this->projection_transform = glm::ortho(left, right, bottom, top, near, far);
}
