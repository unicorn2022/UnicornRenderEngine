#pragma once
#include "engine/mesh/MeshSquare.h"

MeshSquare::MeshSquare() {
    CreateSquare();
}
MeshSquare::~MeshSquare() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void MeshSquare::Draw(unsigned int num) {
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
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, num);
}

void MeshSquare::CreateSquare() {
    /* 按照逆时针方向定义正方形的两个三角形 */
    Vertex vertices[4] = {
        //---- 位置 ----         ---- 法线 ----       -- 纹理坐标 --    ---- 切线 ----         ---- 副切线 ----
        {{-1.0f, -1.0f, 0.0f},  {0.0f, 0.0f, 1.0f},   {0.0f, 0.0f},   {1.0f, 0.0f, 0.0f},   {0.0f, 1.0f, 0.0f}},   // 左下
        {{1.0f, -1.0f, 0.0f},   {0.0f, 0.0f, 1.0f},   {1.0f, 0.0f},   {1.0f, 0.0f, 0.0f},   {0.0f, 1.0f, 0.0f}},   // 右下
        {{1.0f,  1.0f, 0.0f},   {0.0f, 0.0f, 1.0f},   {1.0f, 1.0f},   {1.0f, 0.0f, 0.0f},   {0.0f, 1.0f, 0.0f}},   // 右上
        {{-1.0f,  1.0f, 0.0f},  {0.0f, 0.0f, 1.0f},   {0.0f, 1.0f},   {1.0f, 0.0f, 0.0f},   {0.0f, 1.0f, 0.0f}},   // 左上
    };
    unsigned int indices[] = {
        0, 1, 2, // 第一个三角形
        2, 3, 0  // 第二个三角形
    };

    /* 1. 绑定顶点数组VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* 2. 传输顶点数据VBO */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /* 3. 传输索引数据EBO */
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
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
    // 4.4 切线方向
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // 4.5 副切线方向
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    
    /* 5. 取消绑定 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}