#include "rendering/ShaderManager.h"
#include "rendering/ShaderObject.h"
#include "rendering/ShaderProgram.h"
#include "glad/glad.h"
#include <iostream>

// Initialize the static singleton instance
LSYS::Rendering::ShaderManager* LSYS::Rendering::ShaderManager::sShaderManager = nullptr;

namespace LSYS
{
    namespace Rendering
    {

        // Singleton instance getter
        ShaderManager* ShaderManager::Get()
        {
            if (!sShaderManager)
                {
                sShaderManager = new ShaderManager();
            }
            return sShaderManager;
        }

        // Constructor
        ShaderManager::ShaderManager()
        {
            // Initialize maps or other data structures if necessary
        }

        // Destructor
        ShaderManager::~ShaderManager()
        {
            ShaderPrograms.clear();
            ShaderObjects.clear();
        }

        // Initialize resources if necessary
        void ShaderManager::Initialize()
        {
            std::cout << "ShaderManager initialized." << std::endl;
        }

        // Load or retrieve a ShaderObject
        std::shared_ptr<ShaderObject> ShaderManager::LoadShader(const std::string &ShaderName, GLenum ShaderType)
        {
            // Check if the shader already exists
            if (ShaderObjects.find(ShaderName) != ShaderObjects.end())
            {
                return ShaderObjects[ShaderName];
            }

            // Create a new ShaderObject
            auto shader = std::make_shared<ShaderObject>(ShaderName, ShaderType);

            // Store and return the shader
            ShaderObjects[ShaderName] = shader;
            return ShaderObjects[ShaderName];
        }

        // Load or retrieve a ShaderProgram
        std::shared_ptr<ShaderProgram> ShaderManager::LoadShaderProgram(const std::string& ProgramName, const std::string& VertexShaderName, const std::string& FragmentShaderName)
        {
            // Check if the program already exists
            if (ShaderPrograms.find(ProgramName) != ShaderPrograms.end())
            {
                return ShaderPrograms[ProgramName];
            }

            // Retrieve or create the vertex and fragment shaders
            auto vertexShader = LoadShader(VertexShaderName, GL_VERTEX_SHADER);
            auto fragmentShader = LoadShader(FragmentShaderName, GL_FRAGMENT_SHADER);

            if (!vertexShader || !fragmentShader)
            {
                std::cerr << "Failed to load shaders for program: " << ProgramName << std::endl;
                return nullptr;
            }

            // Create a new ShaderProgram
            auto program = std::make_shared<ShaderProgram>(ProgramName);
            program->AttachShaderObject(vertexShader);
            program->AttachShaderObject(fragmentShader);

            if (!program->LinkShaderProgram())
            {
                std::cerr << "Failed to link shader program: " << ProgramName << std::endl;
                return nullptr;
            }

            // Store and return the program
            ShaderPrograms[ProgramName] = program;
            return ShaderPrograms[ProgramName];
        }
    }
}
