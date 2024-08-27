#include "InputSystem.h"

InputSystem& InputSystem::GetInstance(){
    static InputSystem instance;
    return instance;
}

InputSystem::InputSystem() {
    for (int i = 0; i < KEYBOARD_COUNT; i++)
        key_state[i] = KeyState::Released;
}

void InputSystem::UpdateKeyBoardState(GLFWwindow* window){
    bool is_pressed = false;
    /* A-Z: [00, 25] */
    for (int i = 0; i < 26; i++) {
        if (glfwGetKey(window, GLFW_KEY_A + i) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::A + i, is_pressed);
    }
    /* 0-9: [26, 35] */
    for (int i = 0; i < 10; i++) {
        if (glfwGetKey(window, GLFW_KEY_0 + i) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::NUM0 + i, is_pressed);
    }
    /* F1-F12: [36, 47] */
    for (int i = 0; i < 12; i++) {
        if (glfwGetKey(window, GLFW_KEY_F1 + i) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::F1 + i, is_pressed);
    }
    /* Lctrl: 48 */
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::LCtrl, is_pressed);
    }
    /* Rctrl: 49 */
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::RCtrl, is_pressed);
    }
    /* Lshift: 50 */
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::LShift, is_pressed);
    }
    /* Rshift: 51 */
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::RShift, is_pressed);
    }
    /* tab: 52 */
    {
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::TAB, is_pressed);
    }
    /* esc: 53 */
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::ESC, is_pressed);
    }
    /* →←↓↑: [54, 57] */
    for (int i = 0; i < 4; i++) {
        if (glfwGetKey(window, GLFW_KEY_RIGHT + i) == GLFW_PRESS) is_pressed = true;
        else is_pressed = false;
        UpdateKeyState((int)KeyCode::RIGHT + i, is_pressed);
    }
}

KeyState InputSystem::GetKeyState(KeyCode key){
    return key_state[(int)key];
}

void InputSystem::UpdateKeyState(int key_index, bool is_pressed) {
    switch (key_state[key_index]) {
        case KeyState::Released : {
            if (is_pressed) key_state[key_index] = KeyState::First_Pressed;
            else key_state[key_index] = KeyState::Released;
            break;
        }
        case KeyState::First_Pressed : {
            if (is_pressed) key_state[key_index] = KeyState::Pressed;
            else key_state[key_index] = KeyState::First_Released;
            break;
        }
        case KeyState::Pressed : {
            if (is_pressed) key_state[key_index] = KeyState::Pressed;
            else key_state[key_index] = KeyState::First_Released;
            break;
        }
        case KeyState::First_Released : {
            if (is_pressed) key_state[key_index] = KeyState::First_Pressed;
            else key_state[key_index] = KeyState::Released;
            break;
        }    
        default:
            break;
    }
}


