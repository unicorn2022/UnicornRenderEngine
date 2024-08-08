#pragma once
#include "Utils.h"

class Component;
class GO {
public:
    GO(std::string name) {
        this->name = name;
    }
    virtual ~GO() {
        for (auto component : components) delete component;
        components.clear();
    }

public:
    template<typename ComponentType>
    ComponentType* GetComponent() const {
        for (auto component : components) 
            if (dynamic_cast<ComponentType*>(component) != NULL)
                return dynamic_cast<ComponentType*>(component);
        return NULL;
    }

    void AddComponent(Component* component) {
        components.push_back(component);
    }

    std::string GetName() const {
        return name;
    }

private:
    std::vector<Component*> components;
    std::string name;
};