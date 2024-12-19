//
// Created by Ryanc on 11/22/2024.
//

#pragma once

class LSystem;
class GLFWwindow;

class UIManager
{
public:
    void Init(GLFWwindow* window);
    void BeginFrame();
    void EndFrame();
    void Shutdown();

    void (*UpdateCallback)();

    void SetUpdateCallback(void (*UpdateCallbackSignature)());

    void DrawSystemMenu(LSystem* ActiveSystem);
    void DrawMainMenuBar();


    void UpdateScale(float NewScale);

private:

};

