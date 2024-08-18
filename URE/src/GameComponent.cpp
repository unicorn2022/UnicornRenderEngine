#include "GameComponent.h"
#include "engine/gameobject/GO.h"

GameComponent& GameComponent::GetInstance() {
    static GameComponent instance;
    return instance;
}

/* 添加组件 */
void GameComponent::AddComponent(Component* component) {
    if (dynamic_cast<ComponentBorder*>(component) != NULL) {
        component_borders.push_back(dynamic_cast<ComponentBorder*>(component));
    } 
    else if (dynamic_cast<ComponentCamera*>(component) != NULL) {
        component_cameras.push_back(dynamic_cast<ComponentCamera*>(component));
    } 
    else if (dynamic_cast<ComponentMesh*>(component) != NULL) {
        component_meshs.push_back(dynamic_cast<ComponentMesh*>(component));
    } 
    else if (dynamic_cast<ComponentShadow*>(component) != NULL) {
        component_shadows.push_back(dynamic_cast<ComponentShadow*>(component));
    } 
    else if (dynamic_cast<ComponentTransform*>(component) != NULL) {
        component_transforms.push_back(dynamic_cast<ComponentTransform*>(component));
    } 
    else {
        std::cout << "[ERROR::GameComponent.h::AddComponent()] " << component->gameobject->GetName() << " 组件不是合法类型: " << component->type << "\n";
    }
}

/* 删除组件 */
void GameComponent::DeleteComponent(Component* component) {
    if (dynamic_cast<ComponentBorder*>(component) != NULL) {
        ComponentBorder* component_border = dynamic_cast<ComponentBorder*>(component);
        int index = 0;
        for(; index < component_borders.size(); index++)
            if (component_border == component_borders[index])
                break;
        for (; index < component_borders.size() - 1; index++)
            component_borders[index] = component_borders[index + 1];
        component_borders.pop_back();
    }
    else if (dynamic_cast<ComponentCamera*>(component) != NULL) {
        ComponentCamera* component_camera = dynamic_cast<ComponentCamera*>(component);
        int index = 0;
        for(; index < component_cameras.size(); index++)
            if (component_camera == component_cameras[index])
                break;
        for (; index < component_cameras.size() - 1; index++)
            component_cameras[index] = component_cameras[index + 1];
        component_cameras.pop_back();
    } 
    else if (dynamic_cast<ComponentMesh*>(component) != NULL) {
        ComponentMesh* component_mesh = dynamic_cast<ComponentMesh*>(component);
        int index = 0;
        for(; index < component_meshs.size(); index++)
            if (component_mesh == component_meshs[index])
                break;
        for (; index < component_meshs.size() - 1; index++)
            component_meshs[index] = component_meshs[index + 1];
        component_meshs.pop_back();
    } 
    else if (dynamic_cast<ComponentShadow*>(component) != NULL) {
        ComponentShadow* component_shadow = dynamic_cast<ComponentShadow*>(component);
        int index = 0;
        for(; index < component_shadows.size(); index++)
            if (component_shadow == component_shadows[index])
                break;
        for (; index < component_shadows.size() - 1; index++)
            component_shadows[index] = component_shadows[index + 1];
        component_shadows.pop_back();
    } 
    else if (dynamic_cast<ComponentTransform*>(component) != NULL) {
        ComponentTransform* component_transform = dynamic_cast<ComponentTransform*>(component);
        int index = 0;
        for(; index < component_transforms.size(); index++)
            if (component_transform == component_transforms[index])
                break;
        for (; index < component_transforms.size() - 1; index++)
            component_transforms[index] = component_transforms[index + 1];
        component_transforms.pop_back();
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
std::vector<ComponentShadow*> GameComponent::GetComponentShadow() { 
    return component_shadows; 
}
std::vector<ComponentTransform*> GameComponent::GetComponentTransform() { 
    return component_transforms; 
}
std::vector<ComponentMesh*> GameComponent::GetComponentMesh(Camera* camera, bool sorted) { 
    // 需要对 mesh 进行排序
    if (sorted) {
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