#pragma once
#include "Utils.h"
#include "engine/component/Component.h"

class GO {
public:
    GO(std::string name);
    virtual ~GO();

public:
    template<typename ComponentType>
    std::vector<ComponentType*> GetComponents() const {
        std::vector<ComponentType*> ans;
        for (auto component : components) 
            if (dynamic_cast<ComponentType*>(component) != NULL)
                ans.push_back(dynamic_cast<ComponentType*>(component));
        return ans;
    }

    void AddComponent(Component* component);
    std::string GetName() const;

private:
    std::vector<Component*> components;
    std::string name;
};