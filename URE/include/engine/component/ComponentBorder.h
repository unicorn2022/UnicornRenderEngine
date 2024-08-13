#pragma once
#include "engine/component/ComponentMesh.h"

class ComponentBorder : public Component {
public:
    /** 边框组件: 表示GO的边框
     * \param gameobject 绑定的游戏对象
     * \param component_mesh 对应的component_mesh
     * \param border_color 边框颜色
     */
    ComponentBorder(GO* gameobject, ComponentMesh* component_mesh, glm::vec4 border_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    virtual ~ComponentBorder();

public:
    /* 渲染该网格体 */
    void Draw();

private:
    ComponentMesh* component_mesh;
    Material* material;
    bool is_basic_shape;
};