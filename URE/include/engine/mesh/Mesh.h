#pragma once
#include "Utils.h"

/* Mesh 中仅包含网格体的顶点信息 */
class Mesh {
public:
    virtual ~Mesh() {}
public:
    /** 绘制当前网格体, 绘制之前应先调用 Material->Use()
     * \param num 实例化个数
     */
    virtual void Draw(unsigned int num) = 0;

public:
    unsigned int VAO;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};