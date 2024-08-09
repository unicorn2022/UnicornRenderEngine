#pragma once
#include "Utils.h"
#include "GlobalValue.h"
#include "GameComponent.h"
#include "engine/basic/ALL.h"
#include "engine/mesh/ALL.h"
#include "engine/material/ALL.h"
#include "engine/gameobject/ALL.h"

class GameWorld {
public:
    static GameWorld& GetInstance() {
        static GameWorld instance;
        return instance;
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
        auto camera_component = GameComponent::GetInstance().GetComponentCamera();
        for (auto camera_component : camera_component)
            camera_component->HandleKeyBoard(window, delta_time);
    }
    
    void HandleMouseMovement(float x_offset, float y_offset) {
        auto camera_component = GameComponent::GetInstance().GetComponentCamera();
        for (auto camera_component : camera_component)
            camera_component->HandleMouseMovement(x_offset, y_offset);
    }
    
    void HandleMouseScroll(float y_offset) {
        auto camera_component = GameComponent::GetInstance().GetComponentCamera();
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
    int cnt = 0;
    for (auto position : container_position) {
        GO* container = new GOCube("container" + std::to_string(cnt++),
            new MaterialPhongLight(
                new Texture("container_diffuse.png"), 
                new Texture("container_specular.png")
            )
        );
        container->GetComponent<ComponentTransform>()->TransformTranslate(position);
        container->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
        all_game_object.push_back(container);
    }

    /* 5个窗户 */
    cnt = 0;
    for (auto position : window_position) {
        GO* windows = new GOSquare("windows" + std::to_string(cnt++),
            new MaterialPhongLight(
                new Texture("window.png"), 
                new Texture("window.png")
            ),
            true
        );
        windows->GetComponent<ComponentTransform>()->TransformTranslate(position);
        windows->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.5f));
        all_game_object.push_back(windows);
    }

    /* 6个灯光 */
    cnt = 0;
    for (auto light_data : light_datas) {
        GO* light = new GOLight("light" + std::to_string(cnt++), light_data);
        all_game_object.push_back(light);
        if (dynamic_cast<SpotLight*>(light_data) != NULL)
            spot_light = light->GetComponent<ComponentLight>();
    }

    /* 天空盒 */
    std::string skybox_file_name[6] = {"right", "left", "top", "bottom", "front", "back"};
    skybox = new GOSkybox("skybox", 
        new MaterialSkybox(new TextureCube(skybox_file_name, "jpg"))
    );
}