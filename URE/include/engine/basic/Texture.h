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

    /** 常量颜色纹理
     * \param color 颜色 
     */
    Texture(glm::vec4 color);

    /** 从图片中生成纹理
     * \param file_name 文件名
     * \param sRGB 是否为sRGB纹理
     * \param root_directory 根文件夹路径, 含`/`
     */
    Texture(std::string file_name, bool sRGB = false, std::string root_directory = root_path_texture);
    
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
    /* 是否为sRGB纹理 */
    bool sRGB;

private:
    void CreateTexture();

    void LoadTextureData();
};