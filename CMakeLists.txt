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
    ${PROJECT_SOURCE_DIR}/URE/include
)

# 库文件路径
link_directories(
    ${PROJECT_SOURCE_DIR}/3rd-party/glfw-3.4/lib
    ${PROJECT_SOURCE_DIR}/3rd-party/assimp-5.4.2/lib
)


# 源代码
aux_source_directory(${PROJECT_SOURCE_DIR}/URE/src SRC_ENGINE)
aux_source_directory(${PROJECT_SOURCE_DIR}/URE/src/engine/basic SRC_ENGINE_BASIC)
aux_source_directory(${PROJECT_SOURCE_DIR}/URE/src/engine/component SRC_ENGINE_COMPONENT)
aux_source_directory(${PROJECT_SOURCE_DIR}/URE/src/engine/gameobject SRC_ENGINE_GAMEOBJECT)
aux_source_directory(${PROJECT_SOURCE_DIR}/URE/src/engine/material SRC_ENGINE_MATERIAL)
aux_source_directory(${PROJECT_SOURCE_DIR}/URE/src/engine/mesh SRC_ENGINE_MESH)


# 链接器配置
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/3rd-party/glad/src/glad.c
    ${SRC_ENGINE}
    ${SRC_ENGINE_BASIC}
    ${SRC_ENGINE_COMPONENT}
    ${SRC_ENGINE_GAMEOBJECT}
    ${SRC_ENGINE_MATERIAL}
    ${SRC_ENGINE_MESH}
    main.cpp
)
target_link_libraries(${PROJECT_NAME} glfw3 assimp-vc143-mtd)

