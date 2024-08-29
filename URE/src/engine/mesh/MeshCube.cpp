#include "engine/mesh/MeshCube.h"

MeshCube::MeshCube() {
    CreateCube();
}
MeshCube::~MeshCube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void MeshCube::Draw(unsigned int num) {
    glBindVertexArray(VAO);
    // 1. 传输: 实例化 model 矩阵 
    // note: 通过一个buffer更新model矩阵, 在该buffer中, model矩阵的stride为 sizeof(glm::mat4)
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    // 告诉OpenGL: 在每1次绘制实例时, 需要更新顶点属性5~8
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);
    // 2. 绘制实例
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, num);
}

void MeshCube::CreateCube() {
    /* 按照逆时针方向定义正方体每个面的三角形 */
    Vertex vertices[36] = {
        //---- 位置 ----         ---- 法线 ----       -- 纹理坐标 --    ---- 切线 ----         ---- 副切线 ----
        // 后
        {{-1.0f, -1.0f, -1.0f}, {0.0f,  0.0f, -1.0f},   {0.0f, 0.0f}, {}, {}},
        {{1.0f,  1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},   {1.0f, 1.0f}, {}, {}},
        {{1.0f, -1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},   {1.0f, 0.0f}, {}, {}},
        {{1.0f,  1.0f, -1.0f},  {0.0f,  0.0f, -1.0f},   {1.0f, 1.0f}, {}, {}},
        {{-1.0f, -1.0f, -1.0f}, {0.0f,  0.0f, -1.0f},   {0.0f, 0.0f}, {}, {}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f,  0.0f, -1.0f},   {0.0f, 1.0f}, {}, {}},
        // 前
        {{-1.0f, -1.0f,  1.0f}, {0.0f,  0.0f,  1.0f},   {0.0f, 0.0f}, {}, {}},
        {{1.0f, -1.0f,  1.0f},  {0.0f,  0.0f,  1.0f},   {1.0f, 0.0f}, {}, {}},
        {{1.0f,  1.0f,  1.0f},  {0.0f,  0.0f,  1.0f},   {1.0f, 1.0f}, {}, {}},
        {{1.0f,  1.0f,  1.0f},  {0.0f,  0.0f,  1.0f},   {1.0f, 1.0f}, {}, {}},
        {{-1.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f},   {0.0f, 1.0f}, {}, {}},
        {{-1.0f, -1.0f,  1.0f}, {0.0f,  0.0f,  1.0f},   {0.0f, 0.0f}, {}, {}},
        // 左
        {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}, {}, {}},
        {{-1.0f,  1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 1.0f}, {}, {}},
        {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}, {}, {}},
        {{-1.0f, -1.0f, -1.0f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 1.0f}, {}, {}},
        {{-1.0f, -1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f},  {0.0f, 0.0f}, {}, {}},
        {{-1.0f,  1.0f,  1.0f}, {-1.0f,  0.0f,  0.0f},  {1.0f, 0.0f}, {}, {}},
        // 右
        {{1.0f,  1.0f,  1.0f},  {1.0f,  0.0f,  0.0f},   {1.0f, 0.0f}, {}, {}},
        {{1.0f, -1.0f, -1.0f},  {1.0f,  0.0f,  0.0f},   {0.0f, 1.0f}, {}, {}},
        {{1.0f,  1.0f, -1.0f},  {1.0f,  0.0f,  0.0f},   {1.0f, 1.0f}, {}, {}},
        {{1.0f, -1.0f, -1.0f},  {1.0f,  0.0f,  0.0f},   {0.0f, 1.0f}, {}, {}},
        {{1.0f,  1.0f,  1.0f},  {1.0f,  0.0f,  0.0f},   {1.0f, 0.0f}, {}, {}},
        {{1.0f, -1.0f,  1.0f},  {1.0f,  0.0f,  0.0f},   {0.0f, 0.0f}, {}, {}},
        // 下
        {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f},   {0.0f, 1.0f}, {}, {}},
        {{ 1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f},   {1.0f, 1.0f}, {}, {}},
        {{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f},   {1.0f, 0.0f}, {}, {}},
        {{ 1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f},   {1.0f, 0.0f}, {}, {}},
        {{-1.0f, -1.0f,  1.0f}, {0.0f, -1.0f,  0.0f},   {0.0f, 0.0f}, {}, {}},
        {{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f,  0.0f},   {0.0f, 1.0f}, {}, {}},
        // 上
        {{-1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f},   {0.0f, 1.0f}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f},   {1.0f, 0.0f}, {}, {}},
        {{ 1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f},   {1.0f, 1.0f}, {}, {}},
        {{ 1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f},   {1.0f, 0.0f}, {}, {}},
        {{-1.0f,  1.0f, -1.0f}, {0.0f,  1.0f,  0.0f},   {0.0f, 1.0f}, {}, {}},
        {{-1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f},   {0.0f, 0.0f}, {}, {}},
    };
    for (int face = 0; face < 6; face++) {
        Vertex& v1 = vertices[face * 3 + 0];
        Vertex& v2 = vertices[face * 3 + 1];
        Vertex& v3 = vertices[face * 3 + 2];
        glm::vec3 edge1 = v2.position - v1.position;
        glm::vec3 edge2 = v3.position - v1.position;
        glm::vec2 deltaUV1 = v2.tex_coords - v1.tex_coords;
        glm::vec2 deltaUV2 = v3.tex_coords - v1.tex_coords;
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        // 计算切线&副切线
        glm::vec3 tangent, bitangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
        // 赋值
        for (int index = 0; index < 2; index++) {
            vertices[face * 3 + index].tangent = tangent;
            vertices[face * 3 + index].bitangent = bitangent;
        }
    }


    /* 1. 绑定顶点数组VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    /* 2. 传输顶点数据VBO */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /* 3. 指定顶点属性位置 */
    // 3.1 位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 3.2 法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // 3.3 纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
    // 3.4 切线方向
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // 3.5 副切线方向
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    /* 4. 取消绑定 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}