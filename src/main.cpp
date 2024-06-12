//
// Created by Ryan on 5/22/2024.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// header file includes

///opengl extension loader and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

///glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

///std
#include <cstdlib>
#include <cstdio>

///GLBP
#include "ShaderProgram.h"
#include "lindenmayer.h"
#include "logging.hpp"
#include "LSystemAlphabet.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//initialize the program, calling sub-init functions
bool Init(int argc, char** argv);

//print out program usage
void Usage();

//process program arguments and set variables for initialization
void ProcessArguments(int argc, char** argv);

//sub-initialization functions
bool InitGraphics();
bool InitInput();
bool InitLSystems();

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

//camera

//projection matrix, representing how objects in space are projected onto the screen, i.e. camera lens
glm::mat4 ProjectionMatrix = glm::mat4();

//view matrix, representing the viewers transform in space (camera transform)
glm::mat4 ViewMatrix = glm::mat4();

//vp matrix representing camera transform and lens
glm::mat4 ViewProjectionMatrix = glm::mat4();

//view distance from the center of the scene, defaults to 10 but calculated in InitGraphics
double ViewDistance = 10.0;

//Eye location at any given point in time
glm::vec3 EyeLocation;

//center of the bounding box that constrains our model, calculated immediately after model creation
glm::vec3 ModelCenter = glm::vec3(0);

//up direction, always pointing in positive Y
glm::vec3 UpDirection(0.0, 1.0, 0.0);

//axes rendering

const float AxisSize = 5.0f;
const glm::vec3 SceneOrigin(0.0, 0.0, 0.0);
const glm::vec3 SceneForward(1.0, 0.0, 0.0);
const glm::vec3 SceneRight(0.0, 1.0, 0.0);
const glm::vec3 SceneUp(0.0, 0.0, 1.0);


glm::vec3 AxisVertices[6] =
{
    SceneOrigin, SceneForward * AxisSize,
    SceneOrigin, SceneRight * AxisSize,
    SceneOrigin, SceneUp * AxisSize
};

glm::vec3 AxisColors[6] =
{
    SceneForward, SceneForward,
    SceneRight, SceneRight,
    SceneUp, SceneUp
};

//rotation speed in radians/s
double FixedRotationSpeed = 0.5;
double ManualRotationSpeed = 5.0;
double XRotation = 0.0;
double YRotation = 0.0;
double ZRotation = 0.0;

bool bLMBDown = false;
bool bLMBHeld = false;
double XWhenLMBPressed = 0.0;
double YWhenLMBPressed = 0.0;

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

LSystem ActiveSystem;
Turtle ActiveTurtle;
ColoredTriangleList* TriangleList = nullptr;

constexpr double FoV_y_degrees = 50;
constexpr double FoV_y = glm::radians(FoV_y_degrees);

//colored triangle vao/vbo, etc.
GLuint ColoredVertexArrayObject;
GLuint ColoredVertexBufferObject_Positions;
GLuint ColoredVertexBufferObject_Colors;

GLuint AxesVAO;
GLuint AxesVBO_Positions;
GLuint AxesVBO_Colors;

//shader objects
ShaderProgram* PassthroughShaderProgram;
ShaderObject* PassthroughVertexShader;
ShaderObject* PassthroughFragmentShader;


LSystemAlphabet Alphabet;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Usage()
{
    LogInfo("usage: lsys [OPTIONS]\n");
    LogInfo("OPTIONS:\n");
    LogInfo("\t-h, --help           Display this help string and exit\n");
    LogInfo("\n");
    LogInfo("\t-x, --axiom          Specify initial string to generate from\n");
    LogInfo("\t-i, --iterations     Specify number of rewriting iterations to perform\n");
    LogInfo("\t          [NOTE] this grows exponentially\n");
    LogInfo("\t-a, --angle          Specify turtle turn angle\n");
    LogInfo("\t-d, --distance       Specify turtle move distance\n");
    LogInfo("\t-L, --load           Specify a file to load an lsystem from\n");
    LogInfo("\t-rs, --resolution    Specify initial window resolution, WidthxHeight\n");
    LogInfo("\t\n");
}

int main(int argc, char** argv)
{
    //this is required to print out properly on windows
    setbuf(stdout, nullptr);

    //initialize the sim
    if(Init(argc, argv))
    {
        //run the sim
        Run();
    }

    //cleanup the sim
    Cleanup();

    return 0;
}

//process program arguments, prepping active L-system for iteration and geometry generation
void ProcessArguments(int argc, char** argv)
{
    for(int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            Usage();
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--axiom") == 0)
        {
            if((i + 1) < argc)
            {
                ActiveSystem.SetAxiom(argv[i + 1]);
                i++;
            }
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0)
        {
            if((i + 1) < argc)
            {
                ActiveSystem.Iterations = strtol(argv[i + 1], nullptr, 10);
                i++;
            }
        }
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--angle") == 0)
        {
            if((i + 1) < argc)
            {
                ActiveSystem.Angle = glm::radians(strtof(argv[i + 1], nullptr));
            }
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--distance") == 0)
        {
            if((i + 1) < argc)
            {
                ActiveSystem.Distance = strtof(argv[i + 1], nullptr);
            }
        }
        else if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--load") == 0)
        {
            if((i+1) < argc)
            {
                ActiveSystem.LoadFromFile(argv[i + 1]);
            }
        }
        else if (strcmp(argv[i], "-rs") == 0 || strcmp(argv[i], "--resolution") == 0)
        {

        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// initialization functions
bool Init(int argc, char** argv)
{
    //process arguments to set variables for the active system
    ProcessArguments(argc, argv);

    LogInfo("initializing...\n");

    if (!InitGraphics())
    {
        return false;
    }

    InitInput();

    printf("\n");

    LogInfo("initialization successful.\n");
    return true;
}

bool InitGraphics()
{
    //attempt initializing GLFW
    if(!(bGLFWInitialized = glfwInit()))
    {
        LogCritical("GLFW initialization failure.\n");
        return false;
    }

    //set error callback
    glfwSetErrorCallback(ErrorCallback);

    //try to set context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    //enable antialiasing
    //glfwWindowHint(GLFW_SAMPLES, 4);

    const char* Title = "GLBP";

    //attempt to create the window
    MainWindow = glfwCreateWindow( Width, Height, Title, nullptr, nullptr);
    if(MainWindow == nullptr)
    {
        LogCritical("Window creation failed.\n");
        return false;
    }

    //make the newly created opengl context current
    glfwMakeContextCurrent(MainWindow);

    //load gl extensions
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        LogCritical("Couldn't load openGL extensions\n");
        return false;
    }
    else
    {
        LogInfo("GLAD loaded GL extensions\n");
    }

    // get version info
    LogInfo("Renderer: %s\n", glGetString(GL_RENDERER));
    LogInfo("OpenGL %s\n", glGetString(GL_VERSION));

    //enable vertical sync
    glfwSwapInterval(1);

    // enable depth testing, making so occluded pixels won't be rendered
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    //create shader objects
    PassthroughShaderProgram = new ShaderProgram();
    PassthroughVertexShader = new ShaderObject("resource/passthrough.vs", GL_VERTEX_SHADER);
    PassthroughFragmentShader = new ShaderObject("resource/passthrough.fs", GL_FRAGMENT_SHADER);

    //compile vert and frag shaders
    PassthroughVertexShader->Compile();
    PassthroughFragmentShader->Compile();

    //attach shaders to the shader program
    PassthroughShaderProgram->Attach(PassthroughVertexShader);
    PassthroughShaderProgram->Attach(PassthroughFragmentShader);

    //link the program
    PassthroughShaderProgram->Link();

    //initialize L systems
    InitLSystems();

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //setup projection matrix
    const double AspectRatio = (double)Width / (double)Height;
    constexpr double zNear = 0.1;
    constexpr double zFar = 1000.0;

    ProjectionMatrix = glm::perspective(FoV_y, AspectRatio, zNear, zFar);

    //set view location, will update every frame for now
    ViewMatrix = glm::lookAt(EyeLocation, glm::vec3(0.0), UpDirection);
    ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;

    glUniformMatrix4fv(glGetUniformLocation(PassthroughShaderProgram->ProgramID, "ViewProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&ViewProjectionMatrix);


    //create vertex buffer for storing per-vertex data
    glGenBuffers(1, &AxesVBO_Positions);
    glBindBuffer(GL_ARRAY_BUFFER, AxesVBO_Positions);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), (GLfloat*)AxisVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &AxesVBO_Colors);
    glBindBuffer(GL_ARRAY_BUFFER, AxesVBO_Colors);
    //glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), TriangleColors, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(glm::vec3), (GLfloat*)AxisColors, GL_STATIC_DRAW);

    //create vertax array object for storing info about bound objects and what to render
    glGenVertexArrays(1, &AxesVAO);
    glBindVertexArray(AxesVAO);

    //specify vertex attribute 0 and specify format
    glBindBuffer(GL_ARRAY_BUFFER, AxesVBO_Positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //specify color layout
    glBindBuffer(GL_ARRAY_BUFFER, AxesVBO_Colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    return true;
}

bool InitLSystems()
{
    //set system angle and distance
    ActiveSystem.Rewrite();
    TriangleList = ActiveTurtle.DrawSystem(ActiveSystem);

    if(TriangleList == nullptr)
    {
        return false;
    }

    constexpr float FLOAT_MIN = 1.175494351E-38;
    constexpr float FLOAT_MAX = 3.402823466E+38;
    glm::vec3 min = glm::vec3(FLOAT_MAX);
    glm::vec3 max = glm::vec3(FLOAT_MIN);
    glm::vec3 center = glm::vec3(0.0);

    LogInfo("loading %d triangles\n", TriangleList->NumTriangles);
    auto* VertLocations = (glm::vec3*) malloc(TriangleList->NumTriangles * sizeof(ColoredTriangle::VertexLocations));
    auto* VertColors= (glm::vec3*) malloc(TriangleList->NumTriangles * sizeof(ColoredTriangle::VertexColors));
    //glm::vec3* VertColors[1024*3];

    for(int i = 0; i < TriangleList->NumTriangles; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            glm::vec3& vert = TriangleList->TriData[i].VertexLocations[j];
            glm::vec3& color = TriangleList->TriData[i].VertexColors[j];

            VertLocations[i * 3 + j] = vert;
            VertColors[i * 3 + j] = color;

            if(vert.x < min.x) min.x = vert.x;
            if(vert.y < min.y) min.y = vert.y;
            if(vert.z < min.z) min.z = vert.z;

            if(vert.x > max.x) max.x = vert.x;
            if(vert.y > max.y) max.y = vert.y;
            if(vert.z > max.z) max.z = vert.z;
        }
    }
    ModelCenter = (min + max) / 2.0f;

    float Distance = glm::length(max.y-min.y);
    ViewDistance = Distance / (2.0f * glm::tan(FoV_y / 2.f)) * 1.25;
    //ViewDistance = ViewDistances[Iterations];
    ViewDistance = 15.0f;

    LogInfo("viewdistance = %f\n", ViewDistance);


    //create vertex buffer for storing per-vertex data
    glGenBuffers(1, &ColoredVertexBufferObject_Positions);
    glBindBuffer(GL_ARRAY_BUFFER, ColoredVertexBufferObject_Positions);
    glBufferData(GL_ARRAY_BUFFER, TriangleList->NumTriangles * sizeof(ColoredTriangle::VertexLocations), (GLfloat*)VertLocations, GL_STATIC_DRAW);

    glGenBuffers(1, &ColoredVertexBufferObject_Colors);
    glBindBuffer(GL_ARRAY_BUFFER, ColoredVertexBufferObject_Colors);
    //glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), TriangleColors, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, TriangleList->NumTriangles * sizeof(ColoredTriangle::VertexColors), (GLfloat*)VertColors, GL_STATIC_DRAW);

    //create vertax array object for storing info about bound objects and what to render
    glGenVertexArrays(1, &ColoredVertexArrayObject);
    glBindVertexArray(ColoredVertexArrayObject);

    //specify vertex attribute 0 and specify format
    glBindBuffer(GL_ARRAY_BUFFER, ColoredVertexBufferObject_Positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    //specify color layout
    glBindBuffer(GL_ARRAY_BUFFER, ColoredVertexBufferObject_Colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    return true;
}

bool InitInput()
{
    //set keyboard callback
    glfwSetKeyCallback(MainWindow, KeyboardEventCallback);

    //set mouse button event callback
    glfwSetMouseButtonCallback(MainWindow, MouseButtonEventCallback);

    //set mouse movement callback
    glfwSetCursorPosCallback(MainWindow, MouseMoveEventCallback);

    //set mouse scroll event callback
    glfwSetScrollCallback(MainWindow, MouseScrollEventCallback);

    //set resize callback
    glfwSetFramebufferSizeCallback(MainWindow, WindowResizeEventCallback);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// main loop

void Run()
{
    LogInfo("run started at time %lfs, running...\n", glfwGetTime());

    while(!bRequestedExit)
    {
        UpdateTiming(MainWindow);
        Tick(DeltaTime);
        Render(DeltaTime);
        ProcessInput();
    }

    LogInfo("running complete.\n");
}

void Tick(double dt)
{
    //EyeLocation = glm::vec3(ViewDistance, 0.0, 0.0) + ModelCenter;
    //EyeLocation = glm::vec3(cos(ThisFrameTime * RotationSpeed) * ViewDistance, 0.0, sin(ThisFrameTime * RotationSpeed) * ViewDistance) + ModelCenter;

    EyeLocation = glm::vec3(cos(XRotation) * ViewDistance, 0.0, sin(XRotation) * ViewDistance) + ModelCenter;


    ViewMatrix = glm::lookAt(EyeLocation, ModelCenter, UpDirection);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

    glUniformMatrix4fv(glGetUniformLocation(PassthroughShaderProgram->ProgramID, "ViewProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&ViewProjectionMatrix);
}

void Render(double dt)
{
    //update uniform variables
    //camera variables
    //timing variables
    //resolution
    //mouse info

    constexpr double Red = 0.0f;
    constexpr double Green = 0.0f;
    constexpr double Blue = 0.0f;

    //set clear color
    glClearColor(Red, Green, Blue, 1.0);

    //clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if(PassthroughShaderProgram != nullptr && glIsProgram(PassthroughShaderProgram->ProgramID))
    {
        //render here
        glUseProgram(PassthroughShaderProgram->ProgramID);
        glBindVertexArray(AxesVAO);
        glDrawArrays(GL_LINES, 0, 6);

        glBindVertexArray(ColoredVertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, TriangleList->NumTriangles * 3);
    }

    //swap front and back buffers
    glfwSwapBuffers(MainWindow);
}

void ProcessInput()
{
    //poll queued events
    glfwPollEvents();

    //request exit if window x has been clicked
    if(glfwWindowShouldClose(MainWindow))
    {
        bRequestedExit = true;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// cleanup functions
void Cleanup()
{
    LogInfo("cleaning up...\n");

    //destroy window if one exists
    if(MainWindow != nullptr)
    {
       glfwDestroyWindow(MainWindow);
    }

    //terminate GLFW
    if(bGLFWInitialized)
    {
       glfwTerminate();
    }

    LogInfo("cleanup complete.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// event callbacks
void ErrorCallback(int error, const char *description)
{
    LogError("Error %X: %s", error, description);
}

void KeyboardEventCallback(GLFWwindow *Window, int KeyCode, int ScanCode, int Action, int Modifiers)
{
    if(Window == nullptr)
    {
        return;
    }

    //set action string
    const char* ActionString = Action == GLFW_PRESS ? "GLFW_PRESS" : Action == GLFW_RELEASE ? "GLFW_RELEASE" : "GLFW_REPEAT";

    LogDebug("KeyCode=%d, ScanCode=%d' Action=%d, Modifiers=%d\n", KeyCode, ScanCode, Action, Modifiers);
    LogDebug("glfwGetKeyScanCode(%d)=%d\n", KeyCode, glfwGetKeyScancode(KeyCode));
    LogDebug("glfwGetKeyName(KeyCode,ScanCode)=%s\n",glfwGetKeyName(KeyCode, ScanCode));

    if(KeyCode == GLFW_KEY_ESCAPE)
    {
        bRequestedExit = true;
    }
    else if(KeyCode == GLFW_KEY_R)
    {
        PassthroughShaderProgram->Reload();
    }
    else if(KeyCode == GLFW_KEY_RIGHT)
    {
        //Iterations += 1;
    }
    else if(KeyCode == GLFW_KEY_LEFT)
    {
    }
}

void MouseMoveEventCallback(GLFWwindow *Window, double xPos, double yPos)
{
    if(!bLMBHeld)
    {
        if(bLMBDown)
        {
            XWhenLMBPressed = xPos;
            YWhenLMBPressed = yPos;
            bLMBHeld = true;
        }
    }
    else
    {
        if(!bLMBDown)
        {
            bLMBHeld = false;
            XWhenLMBPressed = 0.0;
            YWhenLMBPressed = 0.0;
        }
    }

    if(bLMBHeld)
    {
        double xDif = xPos - XWhenLMBPressed;
        double yDif = yPos - YWhenLMBPressed;

        XRotation = xDif * ManualRotationSpeed * 0.0175;
        YRotation = yDif * ManualRotationSpeed * 0.0175;

        printf("xDif:%lf\n", xDif);
        printf("yDif:%lf\n", yDif);
        printf("xRot:%lf\n", XRotation);
        printf("yRot:%lf\n", YRotation);

    }

}

void MouseButtonEventCallback(GLFWwindow *Window, int button, int action, int mods)
{
    //only care about LMB
    if(button != GLFW_MOUSE_BUTTON_LEFT)
    {
        return;
    }

    //updated LMB state
    if(action == GLFW_PRESS)
    {
        bLMBDown = true;
    }
    else if(action == GLFW_RELEASE)
    {
        bLMBDown = false;
    }
}

void MouseScrollEventCallback(GLFWwindow *Window, double xOffset, double yOffset)
{
    const double ScaleOffset = 1.0;
    ViewDistance += yOffset * ScaleOffset;
}

void WindowResizeEventCallback(GLFWwindow *Window, int NewWidth, int NewHeight)
{
    if(Window == nullptr)
    {
        return;
    }

    if(NewWidth <= 0)
    {
        NewWidth = 1;
    }
    if(NewHeight <= 0)
    {
        NewHeight = 1;
    }

    glViewport(0, 0, NewWidth, NewHeight);
    LogInfo("Window resized to %dx%d\n", NewWidth, NewHeight);
}

void UpdateTiming(GLFWwindow* Window)
{
    if(Window == nullptr)
    {
        return;
    }

    DeltaTime = (ThisFrameTime = glfwGetTime()) - LastFrameTime;
    LastFrameTime = ThisFrameTime;

    //update timing counter in the window, 4 times a second
    double TimeSinceLastUpdate = ThisFrameTime - LastTimingUpdateTime;
    if (TimeSinceLastUpdate >= 1.0)
    {
        LastTimingUpdateTime = ThisFrameTime;
        double fps = (double)FrameCount / TimeSinceLastUpdate;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(Window, tmp);
        FrameCount = 0;
    }
    FrameCount++;
}




