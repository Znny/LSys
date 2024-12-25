//
// Created by Ryanc on 11/26/2024.
//

#include "../../include/rendering/RenderableBase.h"
#include "../../include/glad/glad.h"

namespace Rendering
{
    RenderableBase::RenderableBase(const std::string& GivenName)
    {
        Name = GivenName;

        VAO = 1>>31;
        VBO = 1>>31;
        EBO = 1>>31;
    }

    void RenderableBase::SetName(const std::string &NewName)
    {
        Name = NewName;
    }

    void RenderableBase::SetProgram(const std::shared_ptr<Rendering::ShaderProgram>& NewProgram)
    {
        ShaderProgram = NewProgram;
    }

    void RenderableBase::Init()
    {
        //create vertex array object for axes rendering
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        //create vbo for vertex properties
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }
}
