#pragma once
#include "Utils.h"
#include "engine/gameobject/GO.h"
#include "engine/basic/Camera.h"

class Component{
public:
    /** 组件, 依赖于游戏对象而存在
     * \param gameobject 绑定的游戏对象
     */
    Component(GO* gameobject) {
        this->gameobject = gameobject;
        gameobject->AddComponent(this);
    }
    virtual ~Component() {}

public:
    GO* gameobject;
};