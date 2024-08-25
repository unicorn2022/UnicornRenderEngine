#pragma once
#include "engine/component/ComponentTransform.h"
#include "engine/mesh/Mesh.h"
#include "engine/material/Material.h"

class ComponentMesh : public Component {
public:
    /** 网格体组件: 表示GO的形状
     * \param gameobject 绑定的游戏对象
     * \param meshs 网格体信息
     * \param materials 材质信息
     * \param is_debug 是否为调试信息
     * \param is_transport 是否是半透明物体
     */
    ComponentMesh(GO* gameobject, std::vector<Mesh*> meshs, std::vector<Material*> materials, bool is_debug = false, bool is_transport = false);
    virtual ~ComponentMesh();

public:
    /** 使用指定材质渲染该网格体
     * \param material 指定材质 
     */
    void Draw(Material* material = NULL);

    /* 是否为半透明物体 */
    bool IsTransport() const;
    /* 是否为基础物体 */
    bool IsBasicShape() const;
    /* 是否为Debug物体 */
    bool IsDebug() const;

private:
    /** 渲染单个网格体
     * \param mesh 网格体
     * \param material 材质
     */
    void DrawOneMesh(Mesh* mesh, Material* material);


private:
    std::vector<Mesh*> meshs;
    std::vector<Material*> materials;
    bool is_transport;
    bool is_debug;
    unsigned int num;
    unsigned int instanceVBO;   // 实例化顶点数据
};