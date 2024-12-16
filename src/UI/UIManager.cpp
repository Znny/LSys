//
// Created by Ryanc on 11/22/2024.
//
#include "UI/UIManager.h"

#include <lindenmayer/lindenmayer.h>
#include <string>

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

void UIManager::SetUpdateCallback(void(*UpdateCallbackSignature)())
{
    UpdateCallback = UpdateCallbackSignature;
}

void UIManager::DrawPrimaryMenu(LSystem* ActiveSystem)
{
    ImGui::Begin("L-System Configuration"); // Start a new window

    // System Name
    static char systemName[64] = "Default System";
    ImGui::InputText("System Name", ActiveSystem->Name, IM_ARRAYSIZE(systemName));

    // Real-time Update Checkbox
    ImGui::Checkbox("Real-time Update", &bUpdateInRealTime);

    bool bSignificantChangeDetected = false;
    // Iteration Count
    bSignificantChangeDetected |= ImGui::SliderInt("Iteration Count", &ActiveSystem->Iterations, 0, 10);

    // Angle
    bSignificantChangeDetected |= ImGui::SliderFloat("Angle", &ActiveSystem->Angle, 0.0f, 360.0f);

    // Distance
    bSignificantChangeDetected |= ImGui::SliderFloat("Distance", &ActiveSystem->Distance, 0.1f, 10.0f);

    static float v2[2] = {10.f, 100.f};
    ImGui::SliderFloat2("EXAMPLE float2", v2, -100.f, 200.f);

    // Axiom
    static char axiom[64] = "F";
    bSignificantChangeDetected |= ImGui::InputText("Axiom", ActiveSystem->Axiom, IM_ARRAYSIZE(axiom));

    //list rules
    int Count = 0;
    for(LS_RewritingRule& Rule : ActiveSystem->RewritingRules)
    {
        if(Rule.Character != ' ')
        {
            ImGui::Text("Rule %c:", Rule.Character);
            ImGui::SameLine();
            ImGui::PushID(Count);
            if(ImGui::InputText("##replacement", Rule.RString, MaxReplacementLength))
            {
                bSignificantChangeDetected |= true;
            }
            ImGui::PopID();
            Count++;
        }
    }

    //Add rule
    static char NewRuleCharacterBuf = ' ';
    static char NewRuleReplacementBuf[MaxReplacementLength+1] = {' '};
    ImGui::SetNextItemWidth(30);
    ImGui::InputText("Character", &NewRuleCharacterBuf, 2);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputText("Replacement", NewRuleReplacementBuf, MaxReplacementLength);
    if(ImGui::Button("Add Rule"))
    {
        if(ActiveSystem->RewritingRules[(int)NewRuleCharacterBuf].Character != NewRuleCharacterBuf)
        {
            ActiveSystem->AddRule(NewRuleCharacterBuf, NewRuleReplacementBuf);
        }
    }

    // Color Palette (Placeholder)
    ImGui::Text("Color Palette: [Placeholder]");
    ImGui::Button("Choose Colors");

    // Save / Load Buttons
    if (ImGui::Button("Save System"))
    {
        // TODO: Implement save functionality
        ImGui::Text("Save button clicked!");
    }
    ImGui::SameLine(); // Place the "Load" button on the same row
    if (ImGui::Button("Load System"))
    {
        // TODO: Implement load functionality
        ImGui::Text("Load button clicked!");
    }

    if(bSignificantChangeDetected) {
        UpdateCallback();
    }

    ImGui::End(); // End the window
}

void UIManager::UpdateScale(float NewScale)
{
    // Adjust style
    ImGui::GetStyle().ScaleAllSizes(NewScale);

    // Adjust fonts
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = NewScale;

    // Optional: Load a scaled font
    //io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("../resource/font/Roboto-Medium.ttf", 16.0f * NewScale); // Adjust base font size
    ImGui_ImplOpenGL3_CreateFontsTexture(); // Rebuild font texture
}
