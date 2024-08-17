#pragma once
#include "engine/gameobject/GO.h"
#include "engine/material/Material.h"

class GOOBJModel : public GO {
public:
    /** OBJ模型
     * \param name GO名称
     * \param file_name 模型文件名称
     * \param material 模型材质
     * \param num 模型实例化个数
     */
    GOOBJModel(std::string name, std::string file_name, Material* material, unsigned int num = 1, bool is_debug = false);
};