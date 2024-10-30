// Created by Ryan on 6/20/2024.
#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// header file includes

///opengl extension loader and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

///glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

///GLBP
#include "ShaderProgram.h"

//LSYS
#include "Camera.h"
#include "lindenmayer.h"
#include "Transform.h"
#include "Turtle.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//initialize the program, calling sub-init functions
bool Init(int argc, char** argv);

//sub-initialization functions
bool InitGraphics();
bool InitInput();
bool InitLSystems();

//print out program usage
void Usage();

//process program arguments and set variables for initialization
void ProcessArguments(int argc, char** argv);

//run the program, which loops updating time, ticking, rendering, and processing input
void Run();

void UpdateTiming(GLFWwindow* window);
void Tick(double dt);
void Render(double dt);

//poll input events for callback processing
void ProcessInput();

//keyboard event callback function
void KeyboardEventCallback(GLFWwindow* Window, int KeyCode, int ScanCode, int Action, int Modifiers);

//mouse movement event callback function
void MouseMoveEventCallback(GLFWwindow* Window, double xPos, double yPos);
void MouseButtonEventCallback(GLFWwindow* Window, int button, int action, int mods);
void MouseScrollEventCallback(GLFWwindow* Window, double xOffset, double yOffset);

//window resize event callback function
void WindowResizeEventCallback(GLFWwindow* Window, int NewWidth, int NewHeight);

//error callback function
void ErrorCallback(int error, const char* description);

//cleanup the program
void Cleanup();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//main window for the sim
GLFWwindow* MainWindow = nullptr;

//default resolution of the window if none is specified
constexpr int DefaultWidth = 1440;
constexpr int DefaultHeight = 1440;

//width and height to use for the window
static int Width = DefaultWidth;
static int Height = DefaultHeight;

//view matrix, representing the viewers transform in space (camera transform)
//projection matrix, representing how objects in space are projected onto the screen, i.e. camera lens
Camera MainCamera(Width, Height);

//vp matrix representing camera transform and lens
glm::mat4 ActiveViewProjectionMatrix = glm::mat4();

//view distance from the center of the scene, defaults to 10 but calculated in InitGraphics
double ViewDistance = 10.0;

//axes rendering
const float AxisLength = 5.0f;
const glm::vec3 SceneOrigin(0.0, 0.0, 0.0);
const glm::vec3 AxisVertices[6] =
{
        SceneOrigin, Transform::WorldRight * AxisLength,
        SceneOrigin, Transform::WorldUp * AxisLength,
        SceneOrigin, Transform::WorldForward * AxisLength,
};
glm::vec3 AxisColors[6] =
{
        glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0),
        glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
        glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0),
};

//rotation speed in degrees per second
double FixedRotationSpeed = 0.5;
double ManualRotationSpeed = 30.0;

bool bLMBDown = false;
bool bLMBHeld = false;
double PreviousMouseXPosition,
        PreviousMouseYPosition;
//manual rotation inputs, gotten from key inputs
float ManualYawInput = 0.0f;
float ManualPitchInput = 0.0f;
float ManualRollInput = 0.0f;

//timing
static double LastFrameTime = 0;
static double ThisFrameTime = 0;
static double LastTimingUpdateTime = 0;
static double DeltaTime = 0.0;
static unsigned int FrameCount = 0;

//exit flag
static bool bRequestedExit = false;

//initialization flags
static bool bGLFWInitialized = false;

//Active L-System and Active Turtle
LSystem ActiveSystem;
Turtle ActiveTurtle;
ColoredTriangleList* TriangleList = nullptr;

//field of vision
constexpr double FoV_y_degrees = 50;
constexpr double FoV_y = glm::radians(FoV_y_degrees);

//colored triangle vao/vbo etc.
GLuint ColoredVertexArrayObject;
GLuint ColoredVertexBufferObject_Positions;
GLuint ColoredVertexBufferObject_Colors;

//axes triangle vao/vbo etc.
GLuint AxesVAO;
GLuint AxesVBO_Positions;
GLuint AxesVBO_Colors;

//use LSYS::Rendering namespace, which includes ShaderProgram and ShaderObject
using namespace LSYS::Rendering;

//shader objects
ShaderProgram* PassthroughShaderProgram;
ShaderObject* PassthroughVertexShader;
ShaderObject* PassthroughFragmentShader;

