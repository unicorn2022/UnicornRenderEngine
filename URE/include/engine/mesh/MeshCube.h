#pragma once
#include "engine/mesh/Mesh.h"

class MeshCube : public Mesh {
public:
    /* 标准正方体: [-1, 1]^3 */
    MeshCube();

    ~MeshCube();

public:
    virtual void Draw(unsigned int num);


private:
    unsigned int VBO, EBO;

private:
    void CreateCube();
};