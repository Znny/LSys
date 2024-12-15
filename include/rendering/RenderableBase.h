//
// Created by Ryanc on 11/26/2024.
//

#pragma once

#include <memory>
#include <string>

#include <utility/Transform.h>

typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace Rendering
{
    class ShaderProgram;

    class RenderableBase
    {
    public:
        RenderableBase(std::string& GivenName);

        void SetName(std::string& NewName);
        Transform& GetTransform() { return transform; }

        void SetProgram(std::shared_ptr<ShaderProgram>& NewProgram);

        void Init();


    private:
        std::string Name;
        Transform transform;
        std::shared_ptr<ShaderProgram> ShaderProgram;
        GLuint VAO; //Vertex array object
        GLuint VBO; //Vertex buffer object
        GLuint EBO; //Element buffer object
    };
}



///
/// todo:
/// make new system
/// specify system details
/// save/load
///
///
///
///
///
///
///
///
///
///