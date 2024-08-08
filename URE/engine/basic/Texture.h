#pragma once
#include "Utils.h"
#include "GlobalValue.h"

class Texture {
public:
    /** 空纹理
     * \param width 纹理的宽度
     * \param height 纹理的高度
    */
    Texture(int width, int height) {
        this->width = width;
        this->height = height;
        this->name = "";
        this->path = "";
        CreateTexture();
    }

    /** 从图片中生成纹理
     * \param file_name 文件名
     */
    Texture(std::string file_name) {
        this->width = 0;
        this->height = 0;
        this->name = file_name;
        this->path = root_path_texture + file_name;
        CreateTexture();
        LoadTextureData();
    }

    /** 从图片中生成纹理
     * \param root_directory 根文件夹路径, 含`/`
     * \param file_name 文件名
     */
    Texture(std::string root_directory, std::string file_name) {
        this->width = 0;
        this->height = 0;
        this->name = file_name;
        this->path = root_directory + file_name;
        CreateTexture();
        LoadTextureData();
    }

    /** 从图片中生成纹理
     * \param root_directory 根文件夹路径, 含`/`
     * \param name 纹理名称
     * \param type 纹理类型, 即文件后缀名
     */
    Texture(std::string root_directory, std::string name, std::string type) {
        this->width = 0;
        this->height = 0;
        this->name = name;
        this->path = root_directory + name + "." + type;
        CreateTexture();
        LoadTextureData();
    }
    
    ~Texture() {
        glDeleteTextures(1, &ID);
    }

public:
    /** 使用当前纹理
     * \param index 将纹理绑定的编号
     */
    void Use(int index) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

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
    void CreateTexture() {
        /* 1. 绑定纹理ID */
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        /* 2. 为当前绑定的纹理对象设置环绕、过滤方式 */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// S轴环绕方式: 插值到边界
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T轴环绕方式: 插值到边界
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // 缩小过滤方式: 线性
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 放大过滤方式: 线性
        /* 3. 如果已经有宽高信息, 则申请空间 */
        if (width != 0 && height != 0)
            glTexImage2D(
                GL_TEXTURE_2D,      // 纹理目标
                0,                  // MIPMAP级别
                GL_RGBA,            // 将纹理存储为何种形式
                width, height,      // 宽高
                0,                  // 总是0
                GL_RGBA,            // 源图像通道数
                GL_UNSIGNED_BYTE,   // 源图像数据类型
                NULL                // 源图像数据
            );
    }

    void LoadTextureData() {
        /* 加载并生成纹理 */
        int channel;
        unsigned char *data = Utils::ReadPicture(path, width, height, channel);
        if (data != NULL) {
            if (channel == 1) 
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_R8, GL_UNSIGNED_BYTE, data);
            else if (channel == 2)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
            else if (channel == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else if (channel == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
    }


};