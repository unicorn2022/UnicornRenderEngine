#pragma once
#include "engine/gameobject/GO.h"
#include "engine/mesh/MeshSquare.h"
#include "engine/material/post_process/MaterialPostProcess.h"
#include "engine/basic/FrameBuffer.h"

class GOScreen : public GO {
public:
    GOScreen(std::string name);
    ~GOScreen();

public:
    void SetTargetFrameBuffer2D(FrameBuffer2D* frame_buffer);
    void Draw();

private:
    MeshSquare* screen;
    MaterialPostProcess* screen_mat;
};