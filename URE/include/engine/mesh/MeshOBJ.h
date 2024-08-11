#pragma once
#include "engine/mesh/Mesh.h"
#include "engine/basic/Texture.h"

class MeshOBJSubMesh : public Mesh{
public:
    MeshOBJSubMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

    ~MeshOBJSubMesh();

public:
    virtual void Draw();

private:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;       // 顶点
    std::vector<unsigned int> indices;  // 索引

private:
    void CreateOBJSubMesh();
};

class MeshOBJ : public Mesh {
public:
    /** 从文件中加载OBJ模型
     * \param root_path 模型根路径
     * \param name 模型名称
     */
    MeshOBJ(std::string root_path, std::string name);

    ~MeshOBJ();

public:
    virtual void Draw();
private:
    std::vector<MeshOBJSubMesh*> sub_meshs;
    std::vector<Texture*> sub_meshs_diffuse;
    std::vector<Texture*> sub_meshs_specular;
    std::string directory;
    std::string obj_model_path;

private:
    void CreateOBJ();

    /** 处理 assimp 节点
     * \param node assimp 节点
     * \param scene assimp 场景
     */
    void ProcessNode(aiNode* node, const aiScene* scene);

    /** 处理 assimp 网格体, 并将其添加到 vector<Mesh*>meshs 中
     * \param ai_mesh assimp 网格体
     * \param scene assimp 场景
     */
    void ProcessMesh(aiMesh* ai_mesh, const aiScene* scene);

    /** 处理 assimp 纹理, 并将其添加到 TextureManager 中
     * \param mat assimp 材质
     * \param type assimp 纹理类型
     * \return 材质中, 对应类型的第一张纹理
     */
    Texture* ProcessTexture(aiMaterial* mat, aiTextureType type);
};