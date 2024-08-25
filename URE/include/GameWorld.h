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
    void RenderTick();

public:
    void ShowOnlySkybox();
    void ShowAllObject();
    void ResetSkyboxTexture();

private:
    GameWorld();
    ~GameWorld();

public:
    std::vector<GO*> all_game_object;
    GOSkybox* skybox;
    TextureCube* skybox_texture;
    ComponentCamera* main_camera;
    SpotLight* spot_light;

    /* 场景1: 测试场景 */
    ComponentCamera* test_camera;
    ComponentMesh* test_camera_screen;
    
    /* 场景2: 行星带 */
    ComponentTransform* planet;

public:
    void HandleKeyBoard(GLFWwindow* window, float delta_time);
    
    void HandleMouseMovement(float x_offset, float y_offset);
    
    void HandleMouseScroll(float y_offset);
};