#pragma once
#include "Utils.h"
#include "engine/gameobject/ALL.h"
#include "engine/component/ALL.h"

class GameWorld {
public:
    static GameWorld& GetInstance();

public:
    void SceneCreate();
    void GameTick();

private:
    GameWorld();
    ~GameWorld();

public:
    std::vector<GO*> all_game_object;
    GOSkybox* skybox;
    ComponentCamera* main_camera;
    SpotLight* spot_light;

    /* Scene_Test_Capture2D */
    ComponentCamera* test_camera;
    ComponentMesh* test_camera_screen;
    
    /* Scene_Test_Planet */
    ComponentTransform* planet;

public:
    void HandleKeyBoard(GLFWwindow* window, float delta_time);
    
    void HandleMouseMovement(float x_offset, float y_offset);
    
    void HandleMouseScroll(float y_offset);
};