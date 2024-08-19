#include "engine/gameobject/GO.h"
#include "GameComponent.h"

GO::GO(std::string name) {
    this->name = name;
}

GO::~GO() {
    for (auto component : components) {
        GameComponent::GetInstance().DeleteComponent(component);
        delete component;
    }
    components.clear();
}

void GO::AddComponent(Component* component, bool add_to_game_component) {
    components.push_back(component);
    if (add_to_game_component) 
        GameComponent::GetInstance().AddComponent(component);
}

std::string GO::GetName() const {
    return name;
}