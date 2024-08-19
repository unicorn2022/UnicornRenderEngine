#pragma once
#include "Utils.h"

class Camera{
public:
    virtual ~Camera() {}

public:
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    
public:
    glm::vec3 position; // 位置
    glm::vec3 front;    // 向前方向
    glm::vec3 up;       // 向上方向
    glm::mat4 projection_transform;
};

class RoamingCamera : public Camera{
public:
    /* 漫游相机 */
    RoamingCamera();
    virtual ~RoamingCamera();

public:
    void SetPosition(glm::vec3 position);
    void SetYaw(float yaw);
    void SetPitch(float pitch);

private:
    float yaw, pitch;   // 欧拉角

public:
    void UpdateCameraVector();
};

class RoamingCameraPerspective : public RoamingCamera {
public:
    /** 透视投影相机
     * \param aspect 宽高比
     * \param fov 视野, 0~90°
     * \param near 近平面距离
     * \param far 远平面距离
     */
    RoamingCameraPerspective(float aspect, float fov, float near, float far);
    virtual ~RoamingCameraPerspective() {}

public:
    float GetFOV() const;
    void SetFOV(float fov);
private:
    float aspect; // 宽高比
    float fov;    // 视野
    float near;   // 近平面距离
    float far;    // 远平面距离
};

class RoamingCameraOrtho : public RoamingCamera {
public:
    /** 正交投影相机
     * \param left 左
     * \param right 右
     * \param bottom 下
     * \param top 上
     * \param near 近平面距离
     * \param far 远平面距离
     */
    RoamingCameraOrtho(float left, float right, float bottom, float top, float near, float far);
    virtual ~RoamingCameraOrtho() {}

public:
    /** 设置正交投影相机的投影平面
     * \param left 左
     * \param right 右
     * \param bottom 下
     * \param top 上
     */
    void SetOrthoSquare(float left, float right, float bottom, float top);

private:
    float left, right, bottom, top;    // 正交平面的左、右、下、上
    float near;   // 近平面距离
    float far;    // 远平面距离
};