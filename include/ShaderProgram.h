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
            //explicit needed to avoid unnecessary implicit conversions
            explicit ShaderProgram(const char* FilenameToLoad = nullptr);

            GLuint GetProgramID() const
            {
                return ProgramID;
            };

            /** AttachShaderObject
             * @param Object - the shader object to attach to the shader program
             */
            void AttachShaderObject(ShaderObject* Object);

            /** CompileAttachedShaders - attempts to compile all attached shader objects
             * @return true if all shader objects compiled successfully
             */
            bool CompileAttachedShaders();

            /** LinkShaderProgram
             * @return true if linking the shader program was successful
             */
            bool LinkShaderProgram();

            /** ReloadShaderObjects
             * ReloadShaderObjects all shader objects
             */
            void ReloadShaderObjects();

            /** ReProcess
             *  ReProcess(load and compile) all attached shader objects
             *  followed by re-linking the program
             */
            void ReProcess();

        private:
            //name of the shader program
            char* ProgramName;

            //opengl shader program identifier
            GLuint ProgramID;

            //attached shader objects
            std::vector<ShaderObject*> AttachedShaderObjects;
        };

    }
}
