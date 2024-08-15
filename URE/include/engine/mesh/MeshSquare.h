#pragma once
#include "engine/mesh/Mesh.h"

class MeshSquare : public Mesh {
public:
    /* 标准正方形: [-1, 1]^2 */
    MeshSquare();
    ~MeshSquare();

public:
    virtual void Draw(unsigned int num);

private:
    unsigned int VBO, EBO;

private:
    void CreateSquare();
};