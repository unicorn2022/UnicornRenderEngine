#pragma once
#include "Utils.h"
#include "engine/basic/ALL.h"
#include "engine/mesh/Mesh.h"
#include "engine/material/ALL.h"

class Component;
class GO {
public:
    GO(std::string name);
    virtual ~GO();

public:
    template<typename ComponentType>
    ComponentType* GetComponent() const;

    void AddComponent(Component* component);
    std::string GetName() const;

private:
    std::vector<Component*> components;
    std::string name;
};

class Component{
public:
    /** 组件, 依赖于游戏对象而存在
     * \param gameobject 绑定的游戏对象
     */
    Component(GO* gameobject) {
        this->gameobject = gameobject;
    }
    virtual ~Component() {}

public:
    GO* gameobject;
    std::string type;
};