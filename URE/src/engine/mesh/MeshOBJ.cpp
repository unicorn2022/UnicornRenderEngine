#include "engine/mesh/MeshOBJ.h"

MeshOBJSubMesh::MeshOBJSubMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    this->vertices = vertices;
    this->indices  = indices;
    CreateOBJSubMesh();
}

MeshOBJSubMesh::~MeshOBJSubMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void MeshOBJSubMesh::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void MeshOBJSubMesh::CreateOBJSubMesh() {
    /* 1. 绑定顶点数组VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    /* 2. 传输顶点数据VBO */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    /* 3. 传输索引数据EBO */
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    /* 4. 指定顶点属性位置 */
    // 4.1 位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 4.2 法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // 4.3 纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    
    /* 5. 取消绑定 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

MeshOBJ::MeshOBJ(std::string root_path, std::string name) {
    this->directory = root_path + name + "/";
    this->obj_model_path = directory + name + ".obj";
    CreateOBJ();
}

MeshOBJ::~MeshOBJ() {
    for (auto mesh : sub_meshs) delete mesh;
    for (auto texture : sub_meshs_diffuse) delete texture;
    for (auto texture : sub_meshs_specular) delete texture;
    sub_meshs.clear();
    sub_meshs_diffuse.clear();
    sub_meshs_specular.clear();
}

void MeshOBJ::Draw() {
    for (auto mesh : sub_meshs) 
        mesh->Draw();
}

void MeshOBJ::CreateOBJ() {
    /* 加载模型 */
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(obj_model_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "[ERROR::MeshOBJ.h::CreateOBJ()] 模型加载失败: " << importer.GetErrorString() << std::endl;
        return;
    }
    /* 处理节点 */
    ProcessNode(scene->mRootNode, scene);
}

void MeshOBJ::ProcessNode(aiNode* node, const aiScene* scene) {
    // 1. 处理所有的网格体
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }
    // 2. 递归处理子节点
    for (int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void MeshOBJ::ProcessMesh(aiMesh* ai_mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // 1. 处理顶点
    for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++) {
        Vertex vertex;
        // 1.1 位置
        vertex.position.x = ai_mesh->mVertices[i].x;
        vertex.position.y = ai_mesh->mVertices[i].y;
        vertex.position.z = ai_mesh->mVertices[i].z;
        // 1.2 法线
        vertex.normal.x = ai_mesh->mNormals[i].x;
        vertex.normal.y = ai_mesh->mNormals[i].y;
        vertex.normal.z = ai_mesh->mNormals[i].z;
        // 1.3 UV坐标
        if (ai_mesh->mTextureCoords[0]) {
            vertex.tex_coords.x = ai_mesh->mTextureCoords[0][i].x;
            vertex.tex_coords.y = ai_mesh->mTextureCoords[0][i].y;
        } else {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    // 2. 处理索引
    for (int i = 0; i < ai_mesh->mNumFaces; i++) {
        aiFace face = ai_mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // 3. 处理材质
    if (ai_mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[ai_mesh->mMaterialIndex];
        Texture* diffuse = ProcessTexture(mat, aiTextureType_DIFFUSE);
        Texture* specular = ProcessTexture(mat, aiTextureType_SPECULAR);
        sub_meshs_diffuse.push_back(diffuse);
        sub_meshs_specular.push_back(specular);
    } 
    // 4. 保存到meshs中
    MeshOBJSubMesh* mesh = new MeshOBJSubMesh(vertices, indices);
    sub_meshs.push_back(mesh);
}

Texture* MeshOBJ::ProcessTexture(aiMaterial* mat, aiTextureType type) {
    Texture* first_texture = NULL;
    for (int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString file_name;
        mat->GetTexture(type, i, &file_name);
        Texture* texture = new Texture(directory, file_name.C_Str());
        if (first_texture == NULL) first_texture = texture;
    }
    return first_texture;
}