#pragma once
#include "Utils.h"

class TextureCube {
public:
    /** 从图片中生成立方纹理
     * \param root_directory 根文件夹路径, 含`/`
     * \param file_name 文件名(右,左,上,下,前,后)
     * \param type 纹理类型, 即文件后缀名
     */
    TextureCube(std::string root_directory, std::string file_name[6], std::string type) {
        for (int i = 0; i < 6; i++)
            this->path[i] = root_directory + file_name[i] + "." + type;
        CreateTextureCube();
        LoadTextureCubeData();
    }
    
    ~TextureCube() {
        glDeleteTextures(1, &ID);
    }

public:
    /** 使用当前纹理
     * \param index 将纹理绑定的编号
     */
    void Use(int index) {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    }

public:
    /* 纹理ID */
    unsigned int ID;
    /* 纹理路径 */
    std::string path[6];

private:
    void CreateTextureCube() {
        /* 1. 绑定纹理ID */
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        /* 2. 为当前绑定的纹理对象设置环绕、过滤方式 */
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// S轴环绕方式: 插值到边界
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T轴环绕方式: 插值到边界
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);// R轴环绕方式: 插值到边界
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // 缩小过滤方式: 线性
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 放大过滤方式: 线性
    }

    void LoadTextureCubeData() {
        /* 加载并生成纹理 */
        int width, height, channel;
        for (int i = 0; i < 6; i++) {
            unsigned char *data = Utils::ReadPicture(path[i], width, height, channel);
            if (data != NULL) {
                if (channel == 1) 
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R8, width, height, 0, GL_R8, GL_UNSIGNED_BYTE, data);
                else if (channel == 2)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                else if (channel == 3)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                else if (channel == 4)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
        }
    }
};