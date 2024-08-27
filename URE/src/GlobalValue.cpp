#include "GlobalValue.h"

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
void GlobalValue::SwitchValue(std::string name) {
    int value = GetIntValue(name);
    if (value == 0) {
        int_value[name] = 1;
        std::cout << "[INFO] " << name << " 取值为: 1\n";
    } 
    else if (value == 1) {
        int_value[name] = 0;
        std::cout << "[INFO] " << name << " 取值为: 0\n";
    }
    else std::cout << "[ERROR::GlobalValue::SwitchValue()] " << name << " 属性取值不是仅为0/1\n";
    
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
