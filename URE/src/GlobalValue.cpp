#include "GlobalValue.h"

GlobalValue& GlobalValue::GetInstance(){
    static GlobalValue instance;
    return instance;
}

void GlobalValue::SetValue(std::string name, float value, float min, float max) {
    if (value < min) value = min;
    if (value > max) value = max;
    float_value[name] = value;
    std::cout << "[INFO] " << name << " 取值为: " << value << "\n";
}
void GlobalValue::SetValue(std::string name, int value, int min, int max) {
    if (value < min) value = min;
    if (value > max) value = max;
    int_value[name] = value;
    std::cout << "[INFO] " << name << " 取值为: " << value << "\n";
}
void GlobalValue::SwitchValue(std::string name, int state_count) {
    int value = GetIntValue(name);
    int next_value = (value + 1) % state_count;
    int_value[name] = next_value;
    std::cout << "[INFO] " << name << " 取值为: " << next_value << "\n";
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
