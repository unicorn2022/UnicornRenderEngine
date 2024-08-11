#include "Utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::string Utils::ReadFile(std::string path) {
    std::ifstream file;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // 打开文件
        file.open(path);
        // 读取字符流
        std::stringstream stream;
        stream << file.rdbuf();
        // 关闭文件
        file.close();
        // 字符流 => string
        return stream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "[ERROR::Utils.h::ReadFile()] 文件 " << path << " 读取失败: " << e.what() << std::endl;
        return "";
    }
}

unsigned char* Utils::ReadPicture(std::string path, int& width, int& height, int& channel) {
    // 是否翻转Y轴
    stbi_set_flip_vertically_on_load(false);
    // stbi加载图片
    unsigned char *data =  stbi_load(path.c_str(), &width, &height, &channel, 0);
    if (data == NULL) {
        std::cout << "[ERROR::Utils.h::ReadPicture()] 图片读取失败: " << path << std::endl;
    }
    return data;
}

void Utils::FreePicture(unsigned char* data) {
    if (data != NULL) stbi_image_free(data);
}