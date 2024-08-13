#pragma once
#include "engine/component/ALL.h"

class GameComponent {
public:
    static GameComponent& GetInstance();

public:
    /* 添加组件 */
    void AddComponent(Component* component);

    /* 删除组件 */
    void DeleteComponent(Component* component);

    /* 获取所有 camera 组件 */
    std::vector<ComponentCamera*> GetComponentCamera();
    
    /* 获取所有 transform 组件 */
    std::vector<ComponentTransform*> GetComponentTransform();
    
    /* 获取所有 mesh 组件, 按照 不透明->透明(远->近) 进行排序 */
    std::vector<ComponentMesh*> GetComponentMesh(Camera* camera);

private:
    GameComponent();
    ~GameComponent();

private:
    std::vector<ComponentCamera*> component_cameras;
    std::vector<ComponentMesh*> component_meshs;
    std::vector<ComponentTransform*> component_transforms;
};