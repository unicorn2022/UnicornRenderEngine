#pragma once
#include "Utils.h"
#include "GlobalValue.h"

class TextureCube {
public:
    /** 从图片中生成立方纹理
     * \param file_name 文件名(右,左,上,下,前,后)
     * \param type 纹理类型, 即文件后缀名
     * \param root_directory 根文件夹路径, 含`/`
     */
    TextureCube(std::string file_name[6], std::string type, std::string root_directory = root_path_skybox);
    
    ~TextureCube();

public:
    /** 使用当前纹理
     * \param index 将纹理绑定的编号
     */
    void Use(int index);

public:
    /* 纹理ID */
    unsigned int ID;
    /* 纹理路径 */
    std::string path[6];

private:
    void CreateTextureCube();

    void LoadTextureCubeData();
};