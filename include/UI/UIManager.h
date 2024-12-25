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
    static void Init(GLFWwindow* window);
    void SetLightingVariables(glm::vec3* LightLocation,
                              glm::vec3* LightColor,
                              glm::vec3* AmbientColor,
                              float* AmbientStrength);

    static void BeginFrame();

    static void EndFrame();

    static void Shutdown();

    void (*UpdateCallback)() = nullptr;
    void (*LightUpdateCallback)() = nullptr;

    void SetUpdateCallback(void (*UpdateCallbackSignature)());
    void SetLightUpdateCallback(void (*LightUpdateCallbackSignature)());

    void DrawSystemMenu(LSystem* ActiveSystem) const;
    void DrawLightMenu() const;

    static void DrawMainMenuBar();

    static void UpdateScale(float NewScale);

private:
    struct SLightingInfo
    {
        glm::vec3* LightLocation = nullptr;
        glm::vec3* LightColor = nullptr;
        glm::vec3* AmbientColor = nullptr;
        float* AmbientStrength = nullptr;
    } LightingInfo;
};

