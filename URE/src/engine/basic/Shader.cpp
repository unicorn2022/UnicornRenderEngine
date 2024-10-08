#include "engine/basic/Shader.h"

static bool show_shader_error = true;

Shader::Shader(std::string name, bool use_geometry_shader, std::string root_directory){
    this->name = name;
    if (use_geometry_shader) 
        CreateShader(root_directory + name + ".vert", root_directory + name + ".geom", root_directory + name + ".frag");
    else 
        CreateShader(root_directory + name + ".vert", root_directory + name + ".frag");
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

/* 使用着色器 */
void Shader::Use() {
    glUseProgram(ID);
    if(show_shader_error) Utils::Check("[ERROR::Shader::Use()] " + name);
}

/* 设置 uniform 变量值: bool */
void Shader::SetUniform(const std::string& name, bool value) const {
    auto location = glGetUniformLocation(ID, name.c_str());
    if (CheckVarLocation(location, name)) glUniform1i(location, (int)value);
}

/* 设置 uniform 变量值: int */
void Shader::SetUniform(const std::string& name, int value) const {
    auto location = glGetUniformLocation(ID, name.c_str());
    if (CheckVarLocation(location, name)) glUniform1i(location, value);
}

/* 设置 uniform 变量值: float */
void Shader::SetUniform(const std::string& name, float value) const {
    auto location = glGetUniformLocation(ID, name.c_str());
    if (CheckVarLocation(location, name)) glUniform1f(location, value);
}

/* 设置 uniform 变量值: vec3 */
void Shader::SetUniform(const std::string& name, glm::vec3 vec) const {
    auto location = glGetUniformLocation(ID, name.c_str());
    if (CheckVarLocation(location, name)) glUniform3f(location, vec.x, vec.y, vec.z);
}

/* 设置 uniform 变量值: vec4 */
void Shader::SetUniform(const std::string& name, glm::vec4 vec) const {
    auto location = glGetUniformLocation(ID, name.c_str());
    if (CheckVarLocation(location, name)) glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

/* 设置 uniform 变量值: mat4 */
void Shader::SetUniform(const std::string& name, glm::mat4& mat) const {
    auto location = glGetUniformLocation(ID, name.c_str());
    if (CheckVarLocation(location, name)) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::CreateShader(std::string vertex_shader_path, std::string fragment_shader_path) {
    /* 1. 从文件中读取源代码 */
    std::string vertex_shader_source = Utils::ReadFile(vertex_shader_path);
    std::string fragment_shader_source = Utils::ReadFile(fragment_shader_path);
    const char* vertex_shader_code = vertex_shader_source.c_str();
    const char* fragment_shader_code = fragment_shader_source.c_str();

    /* 2.1 顶点着色器 */
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    Check(vertex_shader, "顶点着色器编译失败 [" + vertex_shader_path + "]");
    
    /* 2.2 片段着色器 */
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    Check(fragment_shader, "片段着色器编译失败 [" + fragment_shader_path + "]");
    
    /* 3. 着色器程序 */
    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);
    Check(ID, "着色器链接失败");

    /* 4. 删除无用的着色器 */
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::CreateShader(std::string vertex_shader_path, std::string geometry_shader_path, std::string fragment_shader_path) {
    /* 1. 从文件中读取源代码 */
    std::string vertex_shader_source = Utils::ReadFile(vertex_shader_path);
    std::string geometry_shader_source = Utils::ReadFile(geometry_shader_path);
    std::string fragment_shader_source = Utils::ReadFile(fragment_shader_path);
    const char* vertex_shader_code = vertex_shader_source.c_str();
    const char* geometry_shader_code = geometry_shader_source.c_str();
    const char* fragment_shader_code = fragment_shader_source.c_str();

    /* 2.1 顶点着色器 */
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);
    Check(vertex_shader, "顶点着色器编译失败 [" + vertex_shader_path + "]");
    
    /* 2.2 几何着色器 */
    unsigned int geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry_shader, 1, &geometry_shader_code, NULL);
    glCompileShader(geometry_shader);
    Check(geometry_shader, "几何着色器编译失败 [" + geometry_shader_path + "]");

    /* 2.3 片段着色器 */
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);
    Check(fragment_shader, "片段着色器编译失败 [" + fragment_shader_path + "]");
    
    /* 3. 着色器程序 */
    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, geometry_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);
    Check(ID, "着色器链接失败");

    /* 4. 删除无用的着色器 */
    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);
}

bool Shader::Check(int shaderID, std::string message) const {
    int  success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "[ERROR::Shader.h::Check()] " << message << infoLog << std::endl;
        show_shader_error = false;
    }
    return success;
}

bool Shader::CheckVarLocation(GLint location, const std::string& var_name) const {
    if (location == -1) {
        // if(show_shader_error) std::cout << "[ERROR::Shader.h::CheckVarLocation()] " << name << " 中不存在 " << var_name << " 变量\n" ;
        return false;
    } else return true;
}
