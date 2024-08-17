#include "engine/basic/TextureCube.h"

TextureCube::TextureCube(std::string file_name[6], std::string type,bool sRGB, std::string root_directory) {
    for (int i = 0; i < 6; i++)
        this->path[i] = root_directory + file_name[i] + "." + type;
    this->sRGB = sRGB;
    CreateTextureCube();
    LoadTextureCubeData();
}

TextureCube::~TextureCube() {
    glDeleteTextures(1, &ID);
}

void TextureCube::Use(int index) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void TextureCube::CreateTextureCube() {
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

void TextureCube::LoadTextureCubeData() {
    /* 加载并生成纹理 */
    int width, height, channel;
    for (int i = 0; i < 6; i++) {
        unsigned char *data = Utils::ReadPicture(path[i], width, height, channel);
        if (data != NULL) {
            if (sRGB) {
                if (channel == 1 || channel == 2) 
                    std::cout << "[ERROR::TextureCube::LoadTextureCubeData()] SRGB纹理不支持 " << channel << " 通道纹理\n";
                else if (channel == 3)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                else if (channel == 4)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            } else {
                if (channel == 1) 
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R8, width, height, 0, GL_R8, GL_UNSIGNED_BYTE, data);
                else if (channel == 2)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                else if (channel == 3)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                else if (channel == 4)
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
        }
        Utils::FreePicture(data);
    }
}