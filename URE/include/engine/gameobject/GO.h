#pragma once
#include "Utils.h"
#include "engine/component/Component.h"

class GO {
public:
    GO(std::string name);
    virtual ~GO();

public:
    template<typename ComponentType>
    ComponentType* GetComponent() const {
        for (auto component : components) 
            if (dynamic_cast<ComponentType*>(component) != NULL)
                return dynamic_cast<ComponentType*>(component);
        return NULL;
    }

    void AddComponent(Component* component);
    std::string GetName() const;

private:
    std::vector<Component*> components;
    std::string name;
};