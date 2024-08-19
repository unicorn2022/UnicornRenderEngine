#pragma once
#include "engine/component/Component.h"

class ComponentTransform : public Component {
public:
    /** 变换组件: GO的平移、旋转、缩放
     * \param gameobject 绑定的游戏对象
     * \param parent 父对象的Transform组件
     */
    ComponentTransform(GO* gameobject, ComponentTransform* parent = NULL);
    virtual ~ComponentTransform();

public:
    void SetParent(GO* parent);
    void SetParent(ComponentTransform* parent);
    void SetMoveSpeedClamp(float move_speed_min = 2.5f, float move_speed_max = 8.0f);
    void MoveSpeedUp();
    void MoveSpeedDown();

public:
    /** 平移物体
     * \param direction 平移向量
     */
    void TransformTranslate(glm::vec3 direction);
    
    /** 旋转物体
     * \param angle 绕XYZ轴分别旋转角度(角度值)
     */
    void TransformRotate(glm::vec3 angle);
    
    /** 缩放物体
     * \param rate 绕XYZ轴分别缩放的倍率
     */
    void TransformScale(glm::vec3 rate);

public:
    /* 获取当前GO的位置 */
    glm::vec3 GetPosition() const;
    /* 获取相机GO的偏航角 */ 
    float GetYaw() const;
    /* 获取相机GO的俯仰角 */ 
    float GetPitch() const;

public:
    /* 获取局部坐标系变换(平移+旋转)矩阵 */
    glm::mat4 GetTranslateRotateMatrix() const;
    /* 获取模型变换矩阵 */
    glm::mat4 GetModelMatrix() const;

public:
    /* `WASDQE` 控制 `前后左右下上`移动 */
    void ProcessKeyboard(MovementDirection direction, float deltaTime);

    /* 鼠标控制左右+上下移动 */
    void ProcessMouseMovement(float xoffset, float yoffset);

    glm::vec3 position; // 位置
private:
    glm::vec3 rotate;   // XYZ轴的旋转
    glm::vec3 scale;    // XYZ轴的缩放
    ComponentTransform* parent;
    float move_speed, move_speed_min, move_speed_max;
};