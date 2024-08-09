#pragma once
/* 标准库 */
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

/* OpenGL 库 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* 图片加载 */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* ASSIMP 模型加载 */
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

enum MovementDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE
};

class Utils {
public:
    /**
     * 从文件中读取内容
     * \param path 文件路径
     * \return 读取到的内容
     */
    static std::string ReadFile(std::string path) {
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

    /**
     * 从文件中读取图片
     * \param path 文件路径
     * \return 读取到的stbi图片
     */
    static unsigned char* ReadPicture(std::string path, int& width, int& height, int& channel) {
        // 是否翻转Y轴
        stbi_set_flip_vertically_on_load(false);
        // stbi加载图片
        unsigned char *data =  stbi_load(path.c_str(), &width, &height, &channel, 0);
        if (data == NULL) {
            std::cout << "[ERROR::Utils.h::ReadPicture()] 图片读取失败: " << path << std::endl;
        }
        return data;
    }
};