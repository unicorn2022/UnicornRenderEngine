#pragma once
#include "engine/component/ALL.h"

class GameComponent {
public:
    static GameComponent& GetInstance() {
        static GameComponent instance;
        return instance;
    }

public:
    /* 添加组件 */
    void AddComponent(Component* component) {
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

    /* 获取所有 camera 组件 */
    std::vector<ComponentCamera*> GetComponentCamera() { return component_cameras; }
    
    /* 获取所有 light 组件 */
    std::vector<ComponentLight*> GetComponentLight() { return component_lights; }
    
    /* 获取所有 transform 组件 */
    std::vector<ComponentTransform*> GetComponentTransform() { return component_transforms; }
    
    /* 获取所有 mesh 组件, 按照 不透明->透明(远->近) 进行排序 */
    std::vector<ComponentMesh*> GetComponentMesh(Camera* camera) { 
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

private:
    GameComponent() {}
    ~GameComponent() {
        for (auto component : component_cameras) delete component;
        for (auto component : component_lights) delete component;
        for (auto component : component_meshs) delete component;
        for (auto component : component_transforms) delete component;
        component_cameras.clear();
        component_lights.clear();
        component_meshs.clear();
        component_transforms.clear();
    }

private:
    std::vector<ComponentCamera*> component_cameras;
    std::vector<ComponentLight*> component_lights;
    std::vector<ComponentMesh*> component_meshs;
    std::vector<ComponentTransform*> component_transforms;
};