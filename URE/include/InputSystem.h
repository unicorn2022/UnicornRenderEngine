#pragma once
#include "Utils.h"

/** 监控的按键个数
 * \note A-Z: [00, 25]
 * \note 0-9: [26, 35]
 * \note F1-F12: [36, 47]
 * \note Lctrl: 48
 * \note Rctrl: 49
 * \note Lshift: 50
 * \note Rshift: 51
 * \note tab: 52
 * \note esc: 53
 * \note →←↓↑: [54, 57]
 */ 
static const int KEYBOARD_COUNT = 26 + 12 + 10 + 6 + 4;

enum class KeyState{
    Released,        // 没有被按下
    First_Pressed,   // 刚刚被按下
    Pressed,         // 按下
    First_Released   // 刚刚松开
};
enum class KeyCode{
    A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,    // A-Z: [00, 25]
    NUM0,NUM1,NUM2,NUM3,NUM4,NUM5,NUM6,NUM7,NUM8,NUM9,      // 0-9: [26, 35]
    F1,F2,F3,F4,F5,F6,F7,F8,F9,F10,F11,F12,                 // F1-F12: [36, 47]
    LCtrl,RCtrl,LShift,RShift,TAB,ESC,                      // 特殊字符: [48, 53]
    RIGHT, LEFT, DOWN, UP,                                  // →←↓↑: [54, 57]
};

class InputSystem {
public:
    static InputSystem& GetInstance();

private:
    InputSystem();

public:
    /* 更新所有按键状态 */
    void UpdateKeyBoardState(GLFWwindow* window);

    /** 获取按键状态
     * \param key 按键编号
     */
    KeyState GetKeyState(KeyCode key);

private:
    /** 更新单个按键的状态
     * \param key_index 按键编号
     * \param is_pressed 当前帧是否被按下
     */
    void UpdateKeyState(int key_index, bool is_pressed);
    
    KeyState key_state[KEYBOARD_COUNT];

};