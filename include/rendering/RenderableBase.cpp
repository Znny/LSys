//
// Created by Ryanc on 11/26/2024.
//

#include "RenderableBase.h"
#include "glad/glad.h"

namespace Rendering
{
    RenderableBase::RenderableBase(std::string& GivenName)
    {
        Name = GivenName;

        VBO = 1>>31;
        EBO = 1>>31;
    }


    void RenderableBase::SetName(std::string &NewName)
    {
        Name = NewName;
    }

    void RenderableBase::SetProgram(std::shared_ptr<Rendering::ShaderProgram>& NewProgram)
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
