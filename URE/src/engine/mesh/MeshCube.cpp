#include "engine/mesh/MeshCube.h"

MeshCube::MeshCube() {
    CreateCube();
}
MeshCube::~MeshCube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void MeshCube::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MeshCube::CreateCube() {
    /* 按照逆时针方向定义正方体每个面的三角形 */
    float vertices[] = {
        // ---- 位置 ----        ---- 法线 ----        --纹理坐标--
        // 后
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        // 前
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
        // 左
        -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        // 右
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        // 下
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        // 上
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f
    };
    
    /* 1. 绑定顶点数组VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    /* 2. 传输顶点数据VBO */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /* 3. 指定顶点属性位置 */
    // 3.1 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 3.2 法线
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 3.3 纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    /* 4. 取消绑定 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}