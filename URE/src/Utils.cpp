#include "Utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <random>

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
        // std::cout << "[INFO] 读取文件: " << path << std::endl;
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
    } else {
        // std::cout << "[INFO] 读取图片: " << path << std::endl;
    }
    return data;
}

void Utils::FreePicture(unsigned char* data) {
    if (data != NULL) stbi_image_free(data);
}

void Utils::Debug(std::string msg, glm::vec3 data) {
    printf("%s:\n(%.3f, %.3f, %.3f)\n", msg.c_str(), data.x, data.y, data.z);
}
void Utils::Debug(std::string msg, glm::vec4 data) {
    printf("%s:\n(%.3f, %.3f, %.3f, %.3f)\n", msg.c_str(), data.x, data.y, data.z, data.w);
}
void Utils::Debug(std::string msg, glm::mat4 data) {
    printf("%s:\n", msg.c_str());
    for (int i = 0; i < 4; i++) {
        printf("(");
        for (int j = 0; j < 4; j++) {
            printf("%.3f, ", data[i][j]);
        }
        printf(")\n");
    }
}


int Utils::Random(int L, int R) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(L, R);
    return dist(gen);
}

float Utils::Random(float L, float R) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(L, R);
    return dist(gen);
}

void Utils::Check(std::string msg) {
    auto code = glGetError();
    switch (code) {
    case 0: break;
    case 1280: {
        std::cout << msg << "\t1280:枚举参数不合法\n";
        break;
    }
    case 1281: {
        std::cout << msg << "\t1281:值参数不合法\n";
        break;
    }
    case 1282: {
        std::cout << msg << "\t1282:一个指令的状态对指令的参数不合法\n";
        break;
    }
    case 1283: {
        std::cout << msg << "\t1283:push操作造成栈溢出\n";
        break;
    }
    case 1284: {
        std::cout << msg << "\t1284:pop操作时栈在最低点\n";
        break;
    }
    case 1285: {
        std::cout << msg << "\t1285:内存调用操作无法调用（足够的）内存\n";
        break;
    }
    case 1286: {
        std::cout << msg << "\t1286:读取或写入一个不完整的帧缓冲\n";
        break;
    }
    default: {
        std::cout << msg << "\t" << code << "\n";
        break;
    }
    }
}

glm::vec3 Utils::Convert_Direction_To_Euler(glm::vec3 direction) {
    direction = glm::normalize(direction);
    float nx = direction.x, ny = direction.y, nz = direction.z;
    glm::vec3 rotate;
    rotate.x = 0;
    rotate.y = std::asin(nz) * 180.0 / PI;
    rotate.z = std::atan2(ny, nx) * 180.0 / PI;
    return rotate;
}

bool Utils::IsSameDirection(glm::vec3 dir1, glm::vec3 dir2) {
    dir1 = glm::normalize(dir1);
    dir2 = glm::normalize(dir2);
    float dot = glm::dot(dir1, dir2);
    if (std::abs(std::abs(dot) - 1.0) < 1e-5) return true;
    return false;
}
