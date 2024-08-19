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

namespace Utils {
    /**
     * 从文件中读取内容
     * \param path 文件路径
     * \return 读取到的内容
     */
    std::string ReadFile(std::string path);

    /**
     * 从文件中读取图片
     * \param path 文件路径
     * \return 读取到的stbi图片
     */
    unsigned char* ReadPicture(std::string path, int& width, int& height, int& channel);

    /**
     * 读取图片之后释放内存
     * \param data stbi_image读取到的图片数据
     */
    void FreePicture(unsigned char* data);

    void Debug(std::string msg, glm::vec3 data);
    void Debug(std::string msg, glm::vec4 data);
    void Debug(std::string msg, glm::mat4 data);
    void Check(std::string msg);

    int Random(int L, int R);
    float Random(float L, float R);

    const float PI = 3.1415926535;
};