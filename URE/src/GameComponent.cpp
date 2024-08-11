#include "GameComponent.h"
#include "engine/gameobject/GO.h"

GameComponent& GameComponent::GetInstance() {
    static GameComponent instance;
    return instance;
}

/* 添加组件 */
void GameComponent::AddComponent(Component* component) {
    if (dynamic_cast<ComponentCamera*>(component) != NULL) {
        component_cameras.push_back(dynamic_cast<ComponentCamera*>(component));
    } else if (dynamic_cast<ComponentLight*>(component) != NULL) {
        component_lights.push_back(dynamic_cast<ComponentLight*>(component));
    } else if (dynamic_cast<ComponentMesh*>(component) != NULL) {
        component_meshs.push_back(dynamic_cast<ComponentMesh*>(component));
    } else if (dynamic_cast<ComponentTransform*>(component) != NULL) {
        component_transforms.push_back(dynamic_cast<ComponentTransform*>(component));
    } else {
        std::cout << "[ERROR::GameComponent.h::AddComponent()] " << component->gameobject->GetName() << " 组件不是合法类型: " << component->type << "\n";
    }
}

/* 删除组件 */
void GameComponent::DeleteComponent(Component* component) {
    if (dynamic_cast<ComponentCamera*>(component) != NULL) {
        ComponentCamera* component_camera = dynamic_cast<ComponentCamera*>(component);
        int index = 0;
        for(; index < component_cameras.size(); index++)
            if (component_camera == component_cameras[index])
                break;
        for (; index < component_cameras.size() - 1; index++)
            component_cameras[index] = component_cameras[index + 1];
        component_cameras.pop_back();
    } else if (dynamic_cast<ComponentLight*>(component) != NULL) {
        ComponentLight* component_light = dynamic_cast<ComponentLight*>(component);
        int index = 0;
        for(; index < component_lights.size(); index++)
            if (component_light == component_lights[index])
                break;
        for (; index < component_lights.size() - 1; index++)
            component_lights[index] = component_lights[index + 1];
        component_lights.pop_back();
    } else if (dynamic_cast<ComponentMesh*>(component) != NULL) {
        ComponentMesh* component_mesh = dynamic_cast<ComponentMesh*>(component);
        int index = 0;
        for(; index < component_meshs.size(); index++)
            if (component_mesh == component_meshs[index])
                break;
        for (; index < component_meshs.size() - 1; index++)
            component_meshs[index] = component_meshs[index + 1];
        component_meshs.pop_back();
    } else if (dynamic_cast<ComponentTransform*>(component) != NULL) {
        ComponentTransform* component_transform = dynamic_cast<ComponentTransform*>(component);
        int index = 0;
        for(; index < component_transforms.size(); index++)
            if (component_transform == component_transforms[index])
                break;
        for (; index < component_transforms.size() - 1; index++)
            component_transforms[index] = component_transforms[index + 1];
        component_transforms.pop_back();
    } else {
        std::cout << "[ERROR::GameComponent.h::AddComponent()] " << component->gameobject->GetName() << " 组件不是合法类型: " << component->type << "\n";
    }
}

/* 获取所有 camera 组件 */
std::vector<ComponentCamera*> GameComponent::GetComponentCamera() { 
    return component_cameras; 
}

/* 获取所有 light 组件 */
std::vector<ComponentLight*> GameComponent::GetComponentLight() { 
    return component_lights; 
}

/* 获取所有 transform 组件 */
std::vector<ComponentTransform*> GameComponent::GetComponentTransform() { 
    return component_transforms; 
}

/* 获取所有 mesh 组件, 按照 不透明->透明(远->近) 进行排序 */
std::vector<ComponentMesh*> GameComponent::GetComponentMesh(Camera* camera) { 
    std::sort(component_meshs.begin(), component_meshs.end(), [&](ComponentMesh* &A, ComponentMesh* &B){
        if(A->IsTransport() && B->IsTransport()) {
            float distA = glm::distance(A->gameobject->GetComponent<ComponentTransform>()->GetPosition(), camera->position);
            float distB = glm::distance(B->gameobject->GetComponent<ComponentTransform>()->GetPosition(), camera->position);
            return distA > distB;
        }
        else if (!A->IsTransport() && B->IsTransport()) return true;
        else return false;
    });
    return component_meshs;
}

GameComponent::GameComponent() {}

GameComponent::~GameComponent() {}