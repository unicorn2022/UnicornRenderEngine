#include "GameWorld.h"
#include "GameComponent.h"
#include "engine/material/ALL.h"

GameWorld& GameWorld::GetInstance() {
    static GameWorld instance;
    return instance;
}

GameWorld::~GameWorld() {
    for (auto game_object : all_game_object) delete game_object;
    delete skybox;
}

void GameWorld::HandleKeyBoard(GLFWwindow* window, float delta_time) {
    // 判断相机移动方向
    MovementDirection direction;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        direction = MovementDirection::UP;
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        direction = MovementDirection::DOWN;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        direction = MovementDirection::LEFT;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        direction = MovementDirection::RIGHT;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        direction = MovementDirection::FORWARD;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        direction = MovementDirection::BACKWARD;
    } else {
        direction = MovementDirection::NONE;
    }

    // 更新 main_camera 的位置
    main_camera->gameobject->GetComponent<ComponentTransform>()->ProcessKeyboard(direction, delta_time);
}

void GameWorld::HandleMouseMovement(float x_offset, float y_offset) {
    // 更新 main_camera 的旋转
    main_camera->gameobject->GetComponent<ComponentTransform>()->ProcessMouseMovement(x_offset, y_offset);
}

void GameWorld::HandleMouseScroll(float y_offset) {
    // 更新 main_camera 的视野
    main_camera->ProcessMouseScroll(y_offset);
}

void GameWorld::GameTick() {
    /* 设置聚光源属性 */
    SpotLight* light_data = dynamic_cast<SpotLight*>(spot_light->light_data);
    light_data->position = main_camera->camera->position;
    light_data->direction = main_camera->camera->front;
    /* 更新相机状态 */
    test_camera->gameobject->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0, 1, 0));
    auto camera_component = GameComponent::GetInstance().GetComponentCamera();
        for (auto camera_component : camera_component)
            camera_component->UpdateCameraState();

}

GameWorld::GameWorld() {
    /* 主相机 */
    GO* camera = new GOCamera("main_camera", (float)window_width/(float)window_height, 45, main_camera_znear, main_camera_zfar);
    camera->GetComponent<ComponentTransform>()->TransformTranslate(main_camera_position);
    camera->GetComponent<ComponentTransform>()->TransformRotate(glm::vec3(0.0f, -90.0f, 0.0f));
    all_game_object.push_back(camera);
    main_camera = camera->GetComponent<ComponentCamera>();

    /* 1个屏幕, 绑定副相机的帧缓冲 */
    camera = new GOCamera("camera", (float)window_width/(float)window_height, 89, main_camera_znear, main_camera_zfar);
    all_game_object.push_back(camera);
    test_camera = camera->GetComponent<ComponentCamera>();
    GO* screen = new GOSquare("camera_capture", new MaterialNoLight(camera->GetComponent<ComponentCamera>()->frame_buffer->color_texture) );
    screen->GetComponent<ComponentTransform>()->TransformTranslate(glm::vec3(0, 0, -10));
    screen->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(5, 5, 5));
    all_game_object.push_back(screen);
    GO* camera_cube = new GOCube("main_camera_cube", new MaterialConstantColor(glm::vec4(1, 0, 0, 1)) );
    camera_cube->GetComponent<ComponentTransform>()->SetParent(camera);
    camera_cube->GetComponent<ComponentTransform>()->TransformScale(glm::vec3(0.1, 0.1, 0.1));
    all_game_object.push_back(camera_cube);

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