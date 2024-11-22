//
// Created by Ryanc on 11/22/2024.
//

#pragma once

class GLFWwindow;

class UIManager
{
public:
    void Init(GLFWwindow* window);
    void BeginFrame();
    void EndFrame();
    void Shutdown();

    void DrawPrimaryMenu();

};

