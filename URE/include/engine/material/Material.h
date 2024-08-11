#pragma once
#include "Utils.h"
#include "engine/basic/Shader.h"

/* 每种 Material 唯一对应一个Shader */
class Material {
public:
    virtual ~Material();

public:
    /* 使用当前材质绘制网格体, 使用前应保证材质所需参数已经保存到本对象中 */
    virtual void Use() = 0;
protected:
    Shader* shader;
};