#include "engine/component/ComponentTransform.h"
#include "engine/gameobject/GO.h"

ComponentTransform::ComponentTransform(GO* gameobject, ComponentTransform* parent) : Component(gameobject) {
    this->type = "component_transform";
    this->position = glm::vec3(0.0f);
    this->rotate = glm::vec3(0.0f);
    this->scale = glm::vec3(1.0f);
    this->parent = parent;
    this->move_speed = 2.5;
}
ComponentTransform::~ComponentTransform() {}

void ComponentTransform::SetParent(GO* parent) {
    this->parent = parent->GetComponent<ComponentTransform>();
}
void ComponentTransform::SetParent(ComponentTransform* parent) {
    this->parent = parent;
}

void ComponentTransform::TransformTranslate(glm::vec3 direction) {
    position += direction;
}

void ComponentTransform::TransformRotate(glm::vec3 angle) {
    rotate += angle;
}

void ComponentTransform::TransformScale(glm::vec3 rate) {
    scale *= rate;
}

/* 获取当前GO的位置 */
glm::vec3 ComponentTransform::GetPosition() const { 
    return position; 
}
/* 获取相机GO的偏航角 */ 
float ComponentTransform::GetYaw() const { 
    return rotate.y; 
}   
/* 获取相机GO的俯仰角 */ 
float ComponentTransform::GetPitch() const { 
    return rotate.x; 
}

/* 获取模型变换矩阵 */
glm::mat4 ComponentTransform::GetModelMatrix() const {
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

/* `WASDQE` 控制 `前后左右下上`移动 */
void ComponentTransform::ProcessKeyboard(MovementDirection direction, float deltaTime) {
    if (direction == MovementDirection::NONE) return;

    /* 计算前右上方向 */
    glm::vec3 front, right, up;
    front.x = cos(glm::radians(GetYaw())) * cos(glm::radians(GetPitch()));
    front.y = sin(glm::radians(GetPitch()));
    front.z = sin(glm::radians(GetYaw())) * cos(glm::radians(GetPitch()));
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
void ComponentTransform::ProcessMouseMovement(float xoffset, float yoffset) {
    rotate.y += xoffset;
    rotate.x += yoffset;
    if (rotate.x > 89.0f) rotate.x = 89.0f;
    if (rotate.x < -89.0f) rotate.x = -89.0f;
}
