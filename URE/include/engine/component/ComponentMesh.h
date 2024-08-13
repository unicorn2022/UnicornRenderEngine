#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/component/ComponentLight.h"
#include "engine/mesh/Mesh.h"
#include "engine/material/Material.h"
#include "engine/basic/Camera.h"

class ComponentMesh : public Component {
public:
    /** 网格体组件: 表示GO的形状
     * \param gameobject 绑定的游戏对象
     * \param meshs 网格体信息
     * \param materials 材质信息
     * \param is_transport 是否是半透明物体
     */
    ComponentMesh(GO* gameobject, std::vector<Mesh*> meshs, std::vector<Material*> materials, bool is_transport = false);
    virtual ~ComponentMesh();

public:
    /** 渲染该网格体
     * \param lights 灯光信息
     */
    void Draw(std::vector<ComponentLight*> &lights);

    bool IsTransport() const;

private:
    /** 渲染单个网格体
     * \param lights 灯光信息
     * \param mesh 网格体
     * \param material 材质
     */
    void DrawOneMesh(std::vector<ComponentLight*> &lights, Mesh* mesh, Material* material);


private:
    std::vector<Mesh*> meshs;
    std::vector<Material*> materials;
    bool is_transport;
};