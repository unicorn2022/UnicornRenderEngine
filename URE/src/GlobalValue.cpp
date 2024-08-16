#include "GlobalValue.h"

/* 窗口信息 */
const char* window_name = "UnicornRenderEngine";
const int window_width = 800;
const int window_height = 800;

/* 相机信息 */
const float main_camera_move_speed_min = 2.0f;
const float main_camera_move_speed_max = 20.0f;

/* 颜色信息 */
const glm::vec4 color_background = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
const glm::vec3 color_border = glm::vec3(1.0f, 0.0f, 0.0f);

/* 后期处理效果个数 */
const int num_post_process = 9;

/* 路径信息 */
const std::string root_path = "E:/MyProject/GameEngine/UnicornRenderEngine/URE/";
const std::string root_path_shader = root_path + "shader/";
const std::string root_path_resource = root_path + "resource/";
const std::string root_path_texture = root_path_resource + "texture/";
const std::string root_path_skybox = root_path_resource + "skybox/";
const std::string root_path_model = root_path_resource + "model/";


GlobalValue& GlobalValue::GetInstance(){
    static GlobalValue instance;
    return instance;
}

void GlobalValue::SetValue(std::string name, float value, float min, float max) {
    if (value < min) value = min;
    if (value > max) value = max;
    float_value[name] = value;
}
void GlobalValue::SetValue(std::string name, int value, int min, int max) {
    if (value < min) value = min;
    if (value > max) value = max;
    int_value[name] = value;
}
float GlobalValue::GetFloatValue(std::string name) {
    if (float_value.find(name) == float_value.end())
        float_value[name] = 0;
    return float_value[name];
}   
int GlobalValue::GetIntValue(std::string name) {
    if (int_value.find(name) == int_value.end())
        int_value[name] = 0;
    return int_value[name];
}   
