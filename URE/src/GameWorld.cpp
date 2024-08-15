#include "GameWorld.h"

GameWorld& GameWorld::GetInstance() {
    static GameWorld* instance;
    if (instance == NULL) instance = new GameWorld(), instance->GameInit();
    return *instance;
}

GameWorld::GameWorld() {}

GameWorld::~GameWorld() {
    for (auto game_object : all_game_object) delete game_object;
    delete skybox;
}

void GameWorld::HandleKeyBoard(GLFWwindow* window, float delta_time) {
    // LShift 加快 main_camera 的移动速度
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        main_camera->gameobject->GetComponent<ComponentTransform>()->MoveSpeedUp();
    } 
    // LCtrl 降低 main_camera 的移动速度
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        main_camera->gameobject->GetComponent<ComponentTransform>()->MoveSpeedDown();
    }

    // 判断 main_camera 移动方向
    MovementDirection direction;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        direction = MovementDirection::UP;
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        direction = MovementDirection::DOWN;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        direction = MovementDirection::LEFT;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        direction = MovementDirection::RIGHT;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        direction = MovementDirection::FORWARD;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        direction = MovementDirection::BACKWARD;
    } else {
        direction = MovementDirection::NONE;
    }

    // 更新 main_camera 的位置
    main_camera->gameobject->GetComponent<ComponentTransform>()->ProcessKeyboard(direction, delta_time);
}

void GameWorld::HandleMouseMovement(float x_offset, float y_offset) {
    // 更新 main_camera 的旋转
    main_camera->gameobject->GetComponent<ComponentTransform>()->ProcessMouseMovement(x_offset, y_offset);
}

void GameWorld::HandleMouseScroll(float y_offset) {
    // 更新 main_camera 的视野
    main_camera->ProcessMouseScroll(y_offset);
}

