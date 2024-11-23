//
// Created by ryan on 5/26/24.
//
#pragma once

#include <vector>
#include "glad/glad.h"

namespace LSYS
{
    namespace Rendering
    {
        class ShaderProgram;

        class ShaderObject
        {
            friend class ShaderProgram;
        public:
            explicit ShaderObject(const char* filename = nullptr, GLenum shaderType = GL_VERTEX_SHADER);

            bool Load(const char* filename, GLenum shaderType);

            bool Reload();

            bool Compile();

        protected:
            //openGL shader object ID, used for bindings and attachment
            GLuint ObjectID;

            //type of shader object, valid values are defined by openGL
            GLenum Type;

            //file source was loaded from
            char* Filename;

            //soure of the shader, loaded from a file
            char* ShaderSource;
            std::vector<ShaderProgram*> ProgramsIncludedIn;
        };

    }
}
