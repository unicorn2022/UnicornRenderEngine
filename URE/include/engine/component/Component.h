#pragma once
#include "Utils.h"

class GO;
class Component{
public:
    /** 组件, 依赖于游戏对象而存在, 游戏对象消失则组件消失
     * \param gameobject 绑定的游戏对象
     */
    Component(GO* gameobject);
    virtual ~Component();

public:
    GO* gameobject;
    std::string type;
    bool enable;
};