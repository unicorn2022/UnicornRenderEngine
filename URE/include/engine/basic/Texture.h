#pragma once
#include "Utils.h"
#include "GlobalValue.h"

class Texture {
public:
    /** 空纹理
     * \param width 纹理的宽度
     * \param height 纹理的高度
    */
    Texture(int width, int height);

    /** 从图片中生成纹理
     * \param file_name 文件名
     * \param root_directory 根文件夹路径, 含`/`
     */
    Texture(std::string file_name, std::string root_directory = root_path_texture);
    
    ~Texture();

public:
    /** 使用当前纹理
     * \param index 将纹理绑定的编号
     */
    void Use(int index);

public:
    /* 纹理ID */
    unsigned int ID;
    /* 纹理名称 */
    std::string name;
    /* 纹理路径 */
    std::string path;
    /* 纹理的宽高 */
    int width, height;

private:
    void CreateTexture();

    void LoadTextureData();
};