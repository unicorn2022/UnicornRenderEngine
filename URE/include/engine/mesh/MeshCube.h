#pragma once
#include "engine/mesh/Mesh.h"

class MeshCube : public Mesh {
public:
    /* 标准正方体: [-1, 1]^3 */
    MeshCube();

    ~MeshCube();

public:
    virtual void Draw();


private:
    unsigned int VAO, VBO, EBO;

private:
    void CreateCube();
};