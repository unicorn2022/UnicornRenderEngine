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

    /* 获取所有 border 组件 */
    std::vector<ComponentBorder*> GetComponentBorder();

    /* 获取所有 camera 组件 */
    std::vector<ComponentCamera*> GetComponentCamera();
    
    /** 获取所有 mesh 组件
     * \param camera 观察相机
     * \param sorted 是否按照 不透明->透明(远->近) 进行排序
    */
    std::vector<ComponentMesh*> GetComponentMesh(Camera* camera, bool sorted);

    /* 获取所有 shadow_direct_light 组件 */
    std::vector<ComponentShadowDirectLight*> GetComponentShadowDirectLight();

    /* 获取所有 shadow_point_light 组件 */
    std::vector<ComponentShadowPointLight*> GetComponentShadowPointLight();

    /* 获取所有 transform 组件 */
    std::vector<ComponentTransform*> GetComponentTransform();

private:
    GameComponent();
    ~GameComponent();

private:
    std::vector<ComponentBorder*> component_borders;
    std::vector<ComponentCamera*> component_cameras;
    std::vector<ComponentMesh*> component_meshs;
    std::vector<ComponentShadowDirectLight*> component_shadow_direct_lights;
    std::vector<ComponentShadowPointLight*> component_shadow_point_lights;
    std::vector<ComponentTransform*> component_transforms;
};