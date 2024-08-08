#pragma once
#include "Utils.h"
#include "engine/basic/ALL.h"
#include "engine/mesh/ALL.h"
#include "engine/material/ALL.h"
#include "engine/gameobject/ALL.h"
#include "engine/component/ALL.h"

std::vector<glm::vec3> container_position {
    glm::vec3(-1.0f, 0.0f, -1.0f),
    glm::vec3(2.0f, 0.0f, 0.0f)
};

std::vector<glm::vec3> window_position {
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3( 1.5f, 0.0f, 0.51f),
    glm::vec3( 0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3( 0.5f, 0.0f, -0.6f)
};

Light* light_datas[] = {
    new DirectLight(
        glm::vec3(-0.2f, -1.0f, -0.3f),     // 方向
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.5f, 0.5f, 0.5f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(0.7f,  0.2f,  2.0f),      // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(2.3f, -3.3f, -4.0f),      // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(-4.0f,  2.0f, -12.0f),    // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new PointLight(
        glm::vec3(0.0f,  0.0f, -3.0f),      // 位置
        glm::vec3(0.05f, 0.05f, 0.05f),     // 环境光
        glm::vec3(0.8f, 0.8f, 0.8f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f)         // 高光
    ),
    new SpotLight(
        glm::vec3(0.0f,  0.0f, 0.0f),       // 位置
        glm::vec3(-0.2f, -1.0f, -0.3f),     // 方向
        glm::vec3(0, 0, 0),                 // 环境光
        glm::vec3(1.0f, 1.0f, 1.0f),        // 漫反射
        glm::vec3(1.0f, 1.0f, 1.0f),        // 高光
        12.5f,                              // 内切角
        15.0f                               // 外切角
    )
};

class GameWorld {
public:
    static GameWorld& GetInstance() {
        static GameWorld instance;
        return instance;
    }

public:
    template<typename ComponentType>
    std::vector<ComponentType*> GetAllComponent() {
        std::vector<ComponentType*> components;
        for (auto game_object : all_game_object) {
            auto component = game_object->GetComponent<ComponentType>();
            if (component != NULL)
                components.push_back(component);
        }
        return components;
    }

    /* 对于获取到的Mesh组件, 要按照 不透明->透明(远->近) 进行排序 */
    std::vector<ComponentMesh*> GetAllComponentMesh(Camera* camera = NULL) {
        std::vector<ComponentMesh*> components = GetAllComponent<ComponentMesh>();

        std::sort(components.begin(), components.end(), [&](ComponentMesh* &A, ComponentMesh* &B){
            if(A->IsTransport() && B->IsTransport()) {
                float distA = glm::distance(A->gameobject->GetComponent<ComponentTransform>()->GetPosition(), camera->position);
                float distB = glm::distance(B->gameobject->GetComponent<ComponentTransform>()->GetPosition(), camera->position);
                return distA > distB;
            }
            else if (!A->IsTransport() && B->IsTransport()) return true;
            else return false;
        });
        return components;
    }

public:
    void GameTick();

private:
    GameWorld();
    ~GameWorld() {
        for (auto game_object : all_game_object) delete game_object;
        delete skybox;
    }

public:
    std::vector<GO*> all_game_object;
    GOSkybox* skybox;
    ComponentCamera* main_camera;
    ComponentLight* spot_light;

public:
    void HandleKeyBoard(GLFWwindow* window, float delta_time) {
        auto camera_component = GetAllComponent<ComponentCamera>();
        for (auto camera_component : camera_component)
            camera_component->HandleKeyBoard(window, delta_time);
    }
    
    void HandleMouseMovement(float x_offset, float y_offset) {
        auto camera_component = GetAllComponent<ComponentCamera>();
        for (auto camera_component : camera_component)
            camera_component->HandleMouseMovement(x_offset, y_offset);
    }
    
    void HandleMouseScroll(float y_offset) {
        auto camera_component = GetAllComponent<ComponentCamera>();
        for (auto camera_component : camera_component)
            camera_component->HandleMouseScroll(y_offset);
    }
};

void GameWorld::GameTick() {
    SpotLight* light_data = dynamic_cast<SpotLight*>(spot_light->light_data);
    light_data->position = main_camera->camera->position;
    light_data->direction = main_camera->camera->front;
}

GameWorld::GameWorld() {
    /* 主相机 */
    GO* camera = new GOCamera("main_camera");
    all_game_object.push_back(camera);
    main_camera = camera->GetComponent<ComponentCamera>();

    /* 2个箱子 */
    for (auto position : container_position) {
        GO* container = new GOCube("container",
            new MaterialPhongLight(new Texture("container_diffuse.png"), new Texture("container_specular.png"))
        );
        container->GetComponent<ComponentTransform>()->TransformTranslate(position);
        container->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
        all_game_object.push_back(container);
    }

    /* 5个窗户 */
    for (auto position : window_position) {
        GO* windows = new GOSquare("windows",
            new MaterialPhongLight(new Texture("window.png"), new Texture("window.png")),
            true
        );
        windows->GetComponent<ComponentTransform>()->TransformTranslate(position);
        windows->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
        all_game_object.push_back(windows);
    }

    /* 很多个灯光 */
    for (auto light_data : light_datas) {
        GO* light = new GOLight("light", light_data);
        all_game_object.push_back(light);
        if (dynamic_cast<SpotLight*>(light_data) != NULL)
            spot_light = light->GetComponent<ComponentLight>();
    }

    /* 天空盒 */
    std::string skybox_file_name[6] = {"right", "left", "top", "bottom", "front", "back"};
    skybox = new GOSkybox("skybox", 
        new MaterialSkybox(new TextureCube(root_path_skybox, skybox_file_name, "jpg"))
    );
}