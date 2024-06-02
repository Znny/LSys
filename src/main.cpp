//
// Created by Ryan on 5/22/2024.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// header file includes

///opengl extension loader and glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//glm
#include <glm/glm.hpp>

///std
#include <stdlib.h>
#include <stdio.h>
#include "ShaderProgram.h"
#include "lindenmayer.h"
//#include "matrix_clip_space.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "logging.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Init(int argc, char** argv, char** envp);
bool InitGraphics();
bool InitInput();
bool InitLSystems();

void Run();
void UpdateTiming(GLFWwindow* window);
void Tick(double DeltaTime);
void Render(double DeltaTime);

void ProcessInput();
void KeyboardEventCallback(GLFWwindow* Window, int KeyCode, int ScanCode, int Action, int Modifiers);
void WindowResizeEventCallback(GLFWwindow* Window, int NewWidth, int NewHeight);

void ErrorCallback(int error, const char* description);

void Cleanup();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//main window for the sim
GLFWwindow* MainWindow = nullptr;

constexpr int DefaultWidth = 1440;
constexpr int DefaultHeight = 1440;

static int Width = DefaultWidth;
static int Height = DefaultHeight;

//camera

//projection matrix, representing how objects in space are projected onto the screen
glm::mat4 ProjectionMatrix = glm::mat4();

//view matrix, representing the viewers transform in space
glm::mat4 ViewMatrix = glm::mat4();

//vp matrix, needs to be updated when the projection matrix or view matrix changes
glm::mat4 ViewProjectionMatrix = glm::mat4();


//view distance from the center
double ViewDistance = 10.0;

glm::vec3 EyeLocation;
glm::vec3 ModelCenter;
glm::vec3 UpDirection(0.0, 1.0, 0.0);

//rotation speed in radians/s
double RotationSpeed = 10.0f;

//timing
static double LastFrameTime = 0;
static double ThisFrameTime = 0;
static double LastTimingUpdateTime = 0;
static double DeltaTime = 0.0;
static unsigned int FrameCount = 0;
static unsigned int LastTimingUpdateFrame = 0;

//exit flag
static bool bRequestedExit = false;

//initialization flags
static bool bGLFWInitialized = false;

//vertices of a single triangle
static float TriangleVerts[] =
{
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

//colors for a single triangle
static float TriangleColors[] =
{
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f
};


LSystem TestSystem;
Turtle TestTurtle;
ColoredTriangleList* TriangleList = nullptr;
int MaxTriangles = 0;

constexpr double FoV_y_degrees = 50;
constexpr double FoV_y = glm::radians(FoV_y_degrees);

//default vao, vbo, etc.
GLuint VertexArrayObject;
GLuint VertexBufferObject_Positions;
GLuint VertexBufferObject_Colors;

//colored triangle vao/vbo, etc.
GLuint ColoredVertexArrayObject;
GLuint ColoredVertexBufferObject_Positions;
GLuint ColoredVertexBufferObject_Colors;

//shader objects
ShaderProgram* PassthroughShaderProgram;
ShaderObject* PassthroughVertexShader;
ShaderObject* PassthroughFragmentShader;

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
    //initialize the sim
    if(Init(argc, argv, nullptr))
    {
        //run the sim
        Run();
    }

    //cleanup the sim
    Cleanup();

    return 0;
}

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
                TestSystem.SetAxiom(argv[i+1]);
                i++;
            }
        }
        else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0)
        {
            if((i + 1) < argc)
            {
                TestSystem.Iterations = atoi(argv[i+1]);
                i++;
            }
        }
        else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--angle") == 0)
        {
            if((i + 1) < argc)
            {
                TestSystem.Angle = glm::radians(atof(argv[i + 1]));
            }
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--distance") == 0)
        {
            if((i + 1) < argc)
            {
                TestSystem.Distance = atof(argv[i + 1]);
            }
        }
        else if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--load") == 0)
        {
            if((i+1) < argc)
            {
                TestSystem.LoadFromFile(argv[i+1]);
            }
        }
        else if (strcmp(argv[i], "-rs") == 0 || strcmp(argv[i], "--resolution") == 0)
        {

        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// initialization functions
bool Init(int argc, char** argv, char** envp)
{
    //process arguments to setup
    ProcessArguments(argc, argv);

    fprintf(stdout, "initializing...\n");

    if(!InitGraphics())
    {
        return false;
    }

    if(!InitInput())
    {
        return false;
    }

    fprintf(stdout, "initialization successful.\n");
    return true;
}

bool InitGraphics()
{
    //attempt initializing GLFW
    if(!(bGLFWInitialized = glfwInit()))
    {
        fprintf(stdout, "GLFW initialization failure.\n");
        return false;
    }

    //set error callback
    glfwSetErrorCallback(ErrorCallback);

    //try to set context version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    //enable anti-aliasing?
    //glfwWindowHint(GLFW_SAMPLES, 4);

    const char* Title = "GLBP";

    //attempt to create the window
    MainWindow = glfwCreateWindow( Width, Height, Title, NULL, NULL);
    if(MainWindow == nullptr)
    {
        fprintf(stdout, "Window creation failed.\n");
        return false;
    }

    //make the newly created opengl context current
    glfwMakeContextCurrent(MainWindow);

    //load gl extensions
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        fprintf(stdout, "Couldn't load openGL extensions\n");
        return false;
    }
    else
    {
        fprintf(stdout, "GLAD loaded GL extensions\n");
    }

    // get version info
    fprintf(stdout,"Renderer: %s\n", glGetString(GL_RENDERER));
    fprintf(stdout, "OpenGL %s\n", glGetString(GL_VERSION));

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


    ///////////////////////
    /// initialize rendering objects

    for(int i = 0; i < 9; i++)
    {
        TriangleVerts[i] *= 5.0;
    }

    InitLSystems();

    /*
    //create vertex buffer for storing per-vertex data
    glGenBuffers(1, &VertexBufferObject_Positions);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject_Positions);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), TriangleVerts, GL_STATIC_DRAW);

    glGenBuffers(1, &VertexBufferObject_Colors);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject_Colors);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), TriangleColors, GL_STATIC_DRAW);

    //create vertax array object for storing info about bound objects and what to render
    glGenVertexArrays(1, &VertexArrayObject);
    glBindVertexArray(VertexArrayObject);

    //specify vertex attribute 0 and specify format
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject_Positions);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //specify color layout
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject_Colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    */
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //setup projection matrix
    const double AspectRatio = Width / Height;
    constexpr double zNear = 0.1;
    constexpr double zFar = 1000.0;

    ProjectionMatrix = glm::perspective(FoV_y, AspectRatio, zNear, zFar);

    //set view location, will update every frame for now
    ViewMatrix = glm::lookAt(EyeLocation, glm::vec3(0.0), UpDirection);
    ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;

    glUniformMatrix4fv(glGetUniformLocation(PassthroughShaderProgram->ProgramID, "ViewProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&ViewProjectionMatrix);


    return true;
}

bool InitLSystems()
{
    //set system angle and distance
    TestSystem.Rewrite();
    TriangleList = TestTurtle.DrawSystem(TestSystem);

    if(TriangleList == nullptr)
    {
        return false;
    }

   // 1.175494351 E - 38	3.402823466 E + 38
    constexpr float FLOAT_MIN = 1.175494351E-38;
    constexpr float FLOAT_MAX = 3.402823466E+38;
    glm::vec3 min = glm::vec3(FLOAT_MAX);
    glm::vec3 max = glm::vec3(FLOAT_MIN);
    glm::vec3 center = glm::vec3(0.0);

    fprintf(stdout, "loading %d triangles\n", TriangleList->NumTriangles);
    glm::vec3* VertLocations = (glm::vec3*) malloc(TriangleList->NumTriangles * sizeof(ColoredTriangle::VertexLocations));
    glm::vec3* VertColors= (glm::vec3*) malloc(TriangleList->NumTriangles * sizeof(ColoredTriangle::VertexColors));
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

    fprintf(stdout, "viewdistance = %f\n", ViewDistance);


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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //specify color layout
    glBindBuffer(GL_ARRAY_BUFFER, ColoredVertexBufferObject_Colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    return true;
}

bool InitInput()
{
    //set keyboard callback
    glfwSetKeyCallback(MainWindow, KeyboardEventCallback);

    //set resize callback
    glfwSetFramebufferSizeCallback(MainWindow, WindowResizeEventCallback);

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// main loop

void Run()
{
    fprintf(stdout, "run started at time %lfs, running...\n", glfwGetTime());

    while(!bRequestedExit)
    {
        UpdateTiming(MainWindow);
        Tick(DeltaTime);
        Render(DeltaTime);
        ProcessInput();
    }

    fprintf(stdout, "running complete.\n");
}

void Tick(double dt)
{
    EyeLocation = glm::vec3(cos(ThisFrameTime) * ViewDistance, 0.0, sin(ThisFrameTime) * ViewDistance) + ModelCenter;
    ViewMatrix = glm::lookAt(EyeLocation, ModelCenter, UpDirection);
    ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    //fprintf(stdout, "EyeLocation = (%f, %f, %f)\n", EyeLocation.x, EyeLocation.y, EyeLocation.z);
    glUniformMatrix4fv(glGetUniformLocation(PassthroughShaderProgram->ProgramID, "ViewProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&ViewProjectionMatrix);
}

void Render(double dt)
{
    //update uniform variables
    //camera variables
    //timing variables
    //resolution
    //mouse info

    const double Red = 0.0f;
    const double Green = 0.0f;
    const double Blue = 0.0f;

    //set clear color
    glClearColor(Red, Green, Blue, 1.0);

    //clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //if(PassthroughShaderProgram != nullptr && glIsProgram(PassthroughShaderProgram->ProgramID))
    {
        //render here
        glUseProgram(PassthroughShaderProgram->ProgramID);
        //  glBindVertexArray(VertexArrayObject);
        //  glDrawArrays(GL_TRIANGLES, 0, 3);

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
    fprintf(stdout, "cleaning up...\n");

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

    fprintf(stdout, "cleanup complete.\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// event callbacks
void ErrorCallback(int error, const char *description)
{
    ///todo: switch to use bespoke logging once available
    fprintf(stderr, "Error %X: %s", error, description);
}

void KeyboardEventCallback(GLFWwindow *Window, int KeyCode, int ScanCode, int Action, int Modifiers)
{
    if(Action != GLFW_PRESS)
    {
        return;
    }

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

void WindowResizeEventCallback(GLFWwindow *Window, int NewWidth, int NewHeight)
{
    if(NewWidth <= 0)
    {
        NewWidth = 1;
    }
    if(NewHeight <= 0)
    {
        NewHeight = 1;
    }

    glViewport(0, 0, NewWidth, NewHeight);
    fprintf(stdout, "Window resized to %dx%d\n", NewWidth, NewHeight);
}

void UpdateTiming(GLFWwindow* window)
{
    if(window == nullptr)
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
        LastTimingUpdateFrame = FrameCount;
        double fps = (double)FrameCount / TimeSinceLastUpdate;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        FrameCount = 0;
    }
    FrameCount++;
}

