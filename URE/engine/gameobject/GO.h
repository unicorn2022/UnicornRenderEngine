#pragma once
#include "Utils.h"
#include "engine/component/ALL.h"
#include "GameComponent.h"

GO::GO(std::string name) {
    this->name = name;
}

GO::~GO() {}

template<typename ComponentType>
ComponentType* GO::GetComponent() const {
    for (auto component : components) 
        if (dynamic_cast<ComponentType*>(component) != NULL)
            return dynamic_cast<ComponentType*>(component);
    return NULL;
}

void GO::AddComponent(Component* component) {
    components.push_back(component);
    GameComponent::GetInstance().AddComponent(component);
}

std::string GO::GetName() const {
    return name;
}