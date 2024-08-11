#pragma once
#include "engine/component/Component.h"
#include "engine/basic/Light.h"

class ComponentLight : public Component {
public:
    /** 光源组件: 表示GO的自发光
     * \param gameobject 绑定的游戏对象
     * \param light_data 光源数据
     */
    ComponentLight(GO* gameobject, Light* light_data);
    virtual ~ComponentLight();

public:
    Light* light_data;
};