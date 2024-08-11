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
    static std::string ReadFile(std::string path);;

    /**
     * 从文件中读取图片
     * \param path 文件路径
     * \return 读取到的stbi图片
     */
    static unsigned char* ReadPicture(std::string path, int& width, int& height, int& channel);

    /**
     * 读取图片之后释放内存
     * \param data stbi_image读取到的图片数据
     */
    static void FreePicture(unsigned char* data);
};