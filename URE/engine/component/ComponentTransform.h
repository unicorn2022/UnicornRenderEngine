#pragma once
#include "engine/component/Component.h"

class ComponentTransform : public Component {
public:
    /** 变换组件: GO的平移、旋转、缩放
     * \param gameobject 绑定的游戏对象
     * \param parent 父对象的Transform组件
     */
    ComponentTransform(GO* gameobject, ComponentTransform* parent = NULL) : Component(gameobject) {
        this->type = "component_transform";
        this->position = glm::vec3(0.0f);
        this->rotate = glm::vec3(0.0f);
        this->scale = glm::vec3(1.0f);
        this->parent = parent;
        this->move_speed = 2.5;
        this->yaw = -90.0f;
        this->pitch = 0.0f;
    }
    virtual ~ComponentTransform() {}

public:
    /** 平移物体
     * \param direction 平移向量
     */
    virtual void TransformTranslate(glm::vec3 direction) {
        position += direction;
    }
    
    /** 旋转物体
     * \param angle 绕XYZ轴分别旋转角度(角度值)
     */
    virtual void TransformRotate(glm::vec3 angle) {
        rotate += angle;
    }
    
    /** 缩放物体
     * \param rate 绕XYZ轴分别缩放的倍率
     */
    virtual void TransformScale(glm::vec3 rate) {
        scale *= rate;
    }

public:
    glm::vec3 GetPosition() const { return position; }
    float GetYaw() const { return yaw; }
    float GetPitch() const { return pitch;}

public:
    /* 获取模型变换矩阵 */
    glm::mat4 GetModelMatrix() const {
        glm::mat4 model_scale = glm::mat4(1.0f);
        glm::mat4 model_rotate = glm::mat4(1.0f);
        glm::mat4 model_translate = glm::mat4(1.0f);
        // 1. 缩放
        model_scale = glm::scale(model_scale, scale);
        // 2. 旋转
        model_rotate = glm::rotate(model_rotate, glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model_rotate = glm::rotate(model_rotate, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model_rotate = glm::rotate(model_rotate, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
        // 3. 平移
        glm::vec3 move = position;
        ComponentTransform* prt = parent;
        while(prt != NULL) move += prt->position, prt = prt->parent;
        model_translate = glm::translate(model_translate, move);
        return model_translate * model_rotate * model_scale;
    }

public:
    /* `WASDQE` 控制 `前后左右下上`移动 */
    void ProcessKeyboard(MovementDirection direction, float deltaTime) {
        if (direction == MovementDirection::NONE) return;

        /* 计算前右上方向 */
        glm::vec3 front, right, up;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));
        /* 更新位置 */
        float velocity = move_speed * deltaTime;
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

    /* 鼠标控制左右+上下移动 */
    void ProcessMouseMovement(float xoffset, float yoffset) {
        yaw += xoffset;
        pitch += yoffset;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

private:
    glm::vec3 position; // 位置
    glm::vec3 rotate;   // XYZ轴的旋转
    glm::vec3 scale;    // XYZ轴的缩放
    ComponentTransform* parent;
    float move_speed;

private:
    float yaw;      // 相机的偏航角
    float pitch;    // 相机的俯仰角
};