#include "GameComponent.h"
#include "engine/gameobject/GO.h"

GameComponent& GameComponent::GetInstance() {
    static GameComponent instance;
    return instance;
}

/* 添加组件 */
void GameComponent::AddComponent(Component* component) {
    if (component->type == "component_border") {
        component_borders.push_back(dynamic_cast<ComponentBorder*>(component));
    } 
    else if (component->type == "component_camera") {
        component_cameras.push_back(dynamic_cast<ComponentCamera*>(component));
    } 
    else if (component->type == "component_mesh") {
        component_meshs.push_back(dynamic_cast<ComponentMesh*>(component));
    } 
    else if (component->type == "component_shadow_direct_light") {
        component_shadow_direct_lights.push_back(dynamic_cast<ComponentShadowDirectLight*>(component));
    } 
    else if (component->type == "component_shadow_point_light") {
        component_shadow_point_lights.push_back(dynamic_cast<ComponentShadowPointLight*>(component));
    } 
    else if (component->type == "component_transform") {
        component_transforms.push_back(dynamic_cast<ComponentTransform*>(component));
    } 
    else {
        std::cout << "[ERROR::GameComponent.h::AddComponent()] " << component->gameobject->GetName() << " 组件不是合法类型: " << component->type << "\n";
    }
}

/* 删除组件 */
void GameComponent::DeleteComponent(Component* component) {
    if (component->type == "component_border") {
        Utils::RemoveItemInVector(component_borders, dynamic_cast<ComponentBorder*>(component));
    }
    else if (component->type == "component_camera") {
        Utils::RemoveItemInVector(component_cameras, dynamic_cast<ComponentCamera*>(component));
    } 
    else if (component->type == "component_mesh") {
        Utils::RemoveItemInVector(component_meshs, dynamic_cast<ComponentMesh*>(component));
    } 
    else if (component->type == "component_shadow_direct_light") {
        Utils::RemoveItemInVector(component_shadow_direct_lights, dynamic_cast<ComponentShadowDirectLight*>(component));
    } 
    else if (component->type == "component_shadow_point_light") {
        Utils::RemoveItemInVector(component_shadow_point_lights, dynamic_cast<ComponentShadowPointLight*>(component));
    } 
    else if (component->type == "component_transform") {
        Utils::RemoveItemInVector(component_transforms, dynamic_cast<ComponentTransform*>(component));
    } 
    else {
        std::cout << "[ERROR::GameComponent.h::DeleteComponent()] " << component->gameobject->GetName() << " 组件不是合法类型: " << component->type << "\n";
    }
}


std::vector<ComponentBorder*> GameComponent::GetComponentBorder() {
    return component_borders;
}
std::vector<ComponentCamera*> GameComponent::GetComponentCamera() { 
    return component_cameras; 
}
std::vector<ComponentShadowDirectLight*> GameComponent::GetComponentShadowDirectLight() { 
    return component_shadow_direct_lights; 
}
std::vector<ComponentShadowPointLight*> GameComponent::GetComponentShadowPointLight() { 
    return component_shadow_point_lights; 
}
std::vector<ComponentTransform*> GameComponent::GetComponentTransform() { 
    return component_transforms; 
}
std::vector<ComponentMesh*> GameComponent::GetComponentMesh(Camera* camera, bool sorted) { 
    // 需要对 mesh 进行排序
    if (sorted && camera != NULL) {
        std::sort(component_meshs.begin(), component_meshs.end(), [&](ComponentMesh* &A, ComponentMesh* &B){
            if(A->IsTransport() && B->IsTransport()) {
                float distA = glm::distance(A->gameobject->GetComponents<ComponentTransform>()[0]->GetPosition(), camera->position);
                float distB = glm::distance(B->gameobject->GetComponents<ComponentTransform>()[0]->GetPosition(), camera->position);
                return distA > distB;
            }
            else if (!A->IsTransport() && B->IsTransport()) return true;
            else return false;
        });
    }
    return component_meshs;
}

GameComponent::GameComponent() {}

GameComponent::~GameComponent() {}