//
// Created by ryan on 5/26/24.
//
#pragma once

#include "ShaderObject.h"
#include <vector>

namespace LSYS
{
    namespace Rendering
    {

        class ShaderProgram
        {
        public:

            explicit ShaderProgram(const char* name = nullptr);

            void Attach(ShaderObject* Object);

            bool Compile();

            bool Link();

            void Reload();

            GLuint GetProgramID() const;

        protected:
            //name of the shader program
            char* Name;

            //opengl shader program identifier
            GLuint ProgramID;

            //attached shader objects
            std::vector<ShaderObject*> AttachedShaderObjects;
        };

    }
}
