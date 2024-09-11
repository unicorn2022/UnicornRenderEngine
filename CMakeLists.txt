cmake_minimum_required(VERSION 3.23)
# 项目名称
project(UnicornRenderEngine)

# C++标准
set(CMAKE_CXX_STANDARD 11)

# 忽略 warning
if(WIN32)
    add_compile_options(/wd4819)
endif()

# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/3rd-party
    ${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.4/include
    ${PROJECT_SOURCE_DIR}/3rd-party/glad/include
    ${PROJECT_SOURCE_DIR}/3rd-party/assimp-5.4.2/include
    ${PROJECT_SOURCE_DIR}/3rd-party/freetype-2.13.3/include
    ${PROJECT_SOURCE_DIR}/URE/include
)

# 库文件路径
link_directories(
    ${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.4/lib
    ${PROJECT_SOURCE_DIR}/3rd-party/assimp-5.4.2/lib
    ${PROJECT_SOURCE_DIR}/3rd-party/freetype-2.13.3/lib
)

# 递归查询 directory 目录下的所有*.cpp文件, 保存在 variable 中
macro(get_cpp_file_recursively directory variable)
    file(GLOB_RECURSE "${variable}" "${directory}/*.cpp")
endmacro()
get_cpp_file_recursively(${PROJECT_SOURCE_DIR}/URE/src SRC_ENGINE)

# 链接器配置
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/3rd-party/glad/src/glad.c
    ${SRC_ENGINE}
    main.cpp
    scene.cpp
)
target_link_libraries(${PROJECT_NAME} glfw3 assimp-vc143-mtd freetype)

