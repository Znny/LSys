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

void UIManager::SetLightUpdateCallback(void (* LightUpdateCallbackSignature)())
{
    LightUpdateCallback = LightUpdateCallbackSignature;
}

void SliderWithTextInput(const char* label, float* value, float min, float max)
{
    static bool textInputActive = false; // Flag to track if text input is active
    static float textValue = 0.0f;       // Temporary value for text input

    ImGui::PushID(label); // Ensure unique ID for this widget

    // 1. Display the slider
        // Show slider only when not in text input mode
        ImGui::SliderFloat("##slider", value, min, max, "%.3f");

    // 2. Handle double-click to activate text input
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
        textInputActive = true;   // Enable text input
        textValue = *value;       // Initialize with current slider value
        ImGui::OpenPopup("TextInputPopup");
    }

    // 3. Display text input popup
    if (ImGui::BeginPopup("TextInputPopup")) {
        ImGui::Text("Enter Value:");

        // Input box for precise value entry (no EnterReturnsTrue flag)
        if (ImGui::InputFloat("##input", &textValue, 0.0f, 0.0f, "%.3f")) {
            *value = textValue;      // Update slider value immediately
        }

        // Close the popup if the user clicks outside or presses Escape
        if (!ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) || ImGui::IsKeyPressed(ImGuiKey_Escape) || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
            textInputActive = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopID(); // Restore ID stack
}

void UIManager::DrawSystemMenu(LSystem* ActiveSystem)
{
    ImGui::Begin("L-System Configuration"); // Start a new window

    // System Name
    static char systemName[64] = "Default System";
    ImGui::InputText("System Name", ActiveSystem->Name, IM_ARRAYSIZE(systemName));

    bool bSignificantChangeDetected = false;
    // Iteration Count
    bSignificantChangeDetected |= ImGui::SliderInt("Iteration Count", &ActiveSystem->Iterations, 0, 10);

    // Angle
    bSignificantChangeDetected |= ImGui::SliderFloat("Angle", &ActiveSystem->Angle, 0.0f, 360.0f);

    static float TestValue1 = 100;
    SliderWithTextInput("TestValue1", &TestValue1, 0, 360);

    // Distance
    bSignificantChangeDetected |= ImGui::SliderFloat("Distance", &ActiveSystem->Distance, 0.1f, 10.0f);

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

void UIManager::DrawMainMenuBar()
{
    static bool openSaveMenu = false;
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New"))
        {
        }
        if (ImGui::MenuItem("Open"))
        {
        }
        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {
            openSaveMenu = true;
        }

        if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        {
        }
        if (ImGui::MenuItem("Save Alternate", "Ctrl+Alt+S"))
        {
        }
        ImGui::EndMenu();
    }

    if(openSaveMenu)
    {
        ImGui::OpenPopup("Save System");
        openSaveMenu = false;
    }
    // 1. Immediately handle popup after opening
    if (ImGui::BeginPopupModal("Save System", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char saveInput[256] = ""; // Buffer for save input text

        ImGui::Text("Enter a name to save:");
        ImGui::InputText("##SaveInput", saveInput, IM_ARRAYSIZE(saveInput));

        ImGui::Separator();

        if (ImGui::Button("Okay"))
        {
            printf("Saving as: %s\n", saveInput); // Debug message
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::EndMainMenuBar();
}

void UIManager::SetLightingVariables(
        glm::vec3* LightLocation,
        glm::vec3* LightColor,
        glm::vec3* AmbientColor,
        float* AmbientStrength)
{
    LightingInfo.LightLocation = LightLocation;
    LightingInfo.LightColor = LightColor;
    LightingInfo.AmbientColor = AmbientColor;
    LightingInfo.AmbientStrength = AmbientStrength;
}

void UIManager::DrawLightMenu()
{
    ImGui::Begin("Lighting");

    if(ImGui::InputFloat3("Light Location", (float*)LightingInfo.LightLocation))
    {
        LightUpdateCallback();
    }
    if(ImGui::InputFloat3("Light Color", (float*)LightingInfo.LightColor))
    {
        LightUpdateCallback();
    }

    ImGui::InputFloat3("Ambient Color", (float*)LightingInfo.AmbientColor);
    ImGui::InputFloat("Ambient Strength", LightingInfo.AmbientStrength);

    ImGui::End(); // End the window
}


