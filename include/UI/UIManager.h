//
// Created by Ryanc on 11/22/2024.
//

#pragma once

#include "vec3.hpp"

class LSystem;
class GLFWwindow;

class UIManager
{
public:
    void Init(GLFWwindow* window);
    void SetLightingVariables(glm::vec3* LightLocation,
                              glm::vec3* LightColor,
                              glm::vec3* AmbientColor,
                              float* AmbientStrength);
    void BeginFrame();
    void EndFrame();
    void Shutdown();

    void (*UpdateCallback)();
    void (*LightUpdateCallback)();

    void SetUpdateCallback(void (*UpdateCallbackSignature)());
    void SetLightUpdateCallback(void (*LightUpdateCallbackSignature)());

    void DrawSystemMenu(LSystem* ActiveSystem);
    void DrawLightMenu();
    void DrawMainMenuBar();
    void UpdateScale(float NewScale);

private:
    struct SLightingInfo
    {
        glm::vec3* LightLocation = nullptr;
        glm::vec3* LightColor = nullptr;
        glm::vec3* AmbientColor = nullptr;
        float* AmbientStrength = nullptr;
    } LightingInfo;
};

