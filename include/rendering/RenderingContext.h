//
// Created by Ryanc on 12/3/2024.
//

#pragma once
#include <memory>
#include "glm.hpp"

class Camera;
typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace Rendering
{
    class ShaderProgram;

    struct GlobalUniforms
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewProjectionMatrix;
    };

    struct VertexLayout
    {
        glm::vec3 Location;
        glm::vec3 Color;
        glm::vec3 Normals;
    };

    class RenderingContext
    {
    public:
        RenderingContext()=default;

        void Initialize();
        void SetActiveCamera(Camera* Camera);
        void Update();
    private:
        Camera* ActiveCamera = nullptr;
        std::shared_ptr<ShaderProgram> ActiveShaderProgram;
        GLuint ActiveVAO = 1 >> 31;
        GLuint LocationVBO = 1 >> 31;
        GLuint ColorVBO = 1 >> 31;
        GLuint IndexBufferObject = 1 >> 31;
        GLuint UBO_Block = 1 >> 31;

        uint32_t NumVertices = 0;
        uint32_t MaxVertices = 10000000;
    };

}
