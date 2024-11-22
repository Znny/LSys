//
// Created by Ryanc on 11/22/2024.
//
#include "UI/UIManager.h"
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/backends/imgui_impl_glfw.h"
#include "../lib/imgui/backends/imgui_impl_opengl3.h"

void UIManager::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UIManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::DrawPrimaryMenu()
{
    ImGui::Begin("L-System Configuration"); // Start a new window

    // System Name
    static char systemName[128] = "Default System";
    ImGui::InputText("System Name", systemName, IM_ARRAYSIZE(systemName));

    // Real-time Update Checkbox
    static bool realTimeUpdate = true;
    ImGui::Checkbox("Real-time Update", &realTimeUpdate);

    // Iteration Count
    static int iterationCount = 5;
    ImGui::SliderInt("Iteration Count", &iterationCount, 0, 10);

    // Angle
    static float angle = 25.0f;
    ImGui::SliderFloat("Angle", &angle, 0.0f, 360.0f);

    // Distance
    static float distance = 1.0f;
    ImGui::SliderFloat("Distance", &distance, 0.1f, 10.0f);

    // Axiom
    static char axiom[128] = "F";
    ImGui::InputText("Axiom", axiom, IM_ARRAYSIZE(axiom));

    // Rewriting Rules
    static char rules[1024] = "F -> FF+[+F-F-F]-[-F+F+F]";
    ImGui::InputTextMultiline("Rewriting Rules", rules, IM_ARRAYSIZE(rules), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10));

    // Color Palette (Placeholder)
    ImGui::Text("Color Palette: [Placeholder]");
    ImGui::Button("Choose Colors");

    // Save / Load Buttons
    if (ImGui::Button("Save System")) {
        // TODO: Implement save functionality
        ImGui::Text("Save button clicked!");
    }
    ImGui::SameLine(); // Place the "Load" button on the same row
    if (ImGui::Button("Load System")) {
        // TODO: Implement load functionality
        ImGui::Text("Load button clicked!");
    }

    ImGui::End(); // End the window
}
