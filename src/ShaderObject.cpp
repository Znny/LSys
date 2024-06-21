//
// Created by ryan on 5/26/24.
//

#include "ShaderObject.h"

//FILE IO
#include <cstdio>

//memory management
#include <cstdlib>
#include <cstring>
#include "logging.hpp"
#include "ShaderProgram.h"

namespace LSYS
{
    namespace Rendering
    {

        /*** ShaderObject::ShaderObject
         *
         * @param filename - optional filename
         * @param shaderType - optional shadertype
         *
         *  will attempt to load the shaderobject from the given file
         */
        ShaderObject::ShaderObject(const char* filename, GLenum shaderType)
        {
            if (filename != nullptr)
            {
                Load(filename, shaderType);
            }
        }

        /*** ShaderObject::CompileAttachedShaders
         *  Tries to compile the shader, logging errors if they are found
         * @return true if the shader object compiled successfully, false otherwise
         */
        bool ShaderObject::Compile()
        {
            LogInfo("compiling %s...\n",
                    Filename
                    ? Filename
                    : "unknown");

            GLint compile_status = GL_FALSE;
            int info_log_length = 0;
            char info_log[1024];

            //set shader object source and compile, and get compilation results
            glShaderSource(ObjectID, 1, (const GLchar**) (&ShaderSource), NULL);
            glCompileShader(ObjectID);
            glGetShaderiv(ObjectID, GL_COMPILE_STATUS, &compile_status);

            //log shader log if compilation failed
            if (compile_status != GL_TRUE)
            {
                glGetShaderiv(ObjectID, GL_INFO_LOG_LENGTH, &info_log_length);
                glGetShaderInfoLog(ObjectID, info_log_length, NULL, info_log);
                LogError("error: %s\n", info_log);
                return false;
            }

            return true;
        }


        /*** ShaderObject::Load
         *
         * @param filename - filename of the shader object source we wish to load
         * @param ShaderType - the type of shader this file represents, valid options are
         *                   GL_VERTEX_SHADER,
         *                   GL_TESS_CONTROL_SHADER,
         *                   GL_TESS_EVALUATION_SHADER,
         *                   GL_GEOMETRY_SHADER,
         *                   GL_FRAGMENT_SHADER,
         *                   GL_COMPUTE_SHADER
         * @return true if file was successfully loaded and its contents were specified as the shader source
         *
         */
        bool ShaderObject::Load(const char* filename, GLenum ShaderType)
        {
            if (filename == nullptr)
            {
                LogError("Failed to load file, no filename provided\n");
                return false;
            }

            size_t FilenameLength = strlen(filename);
            Filename = (char*) malloc(FilenameLength);
            strcpy(Filename, filename);

            LogVerbose("Opening file \"%s\"\n", Filename);

            FILE* file = fopen(Filename, "r");
            if (!file)
            {
                LogError("Failed to open file %s\n", filename);
                return false;
            }

            // Get file size
            fseek(file, 0, SEEK_END);
            long fileSize = ftell(file);
            fseek(file, 0, SEEK_SET);

            LogVerbose("%s is %d characters long:\n", Filename, fileSize);

            // Allocate memory for shader source
            ShaderSource = (char*) malloc(fileSize + 1);
            if (ShaderSource == nullptr)
            {
                LogError("Failed to allocate memory for shader source\n");
                fclose(file);
                ShaderSource = nullptr;
                return false;
            }

            // Read shader source from file
            size_t BytesRead = fread(ShaderSource, 1, fileSize, file);
            ShaderSource[BytesRead] = '\0'; // Null-terminate the string

            LogVerbose("Read %d Bytes\n", BytesRead);
            LogVerbose("Contents:\n%s\n", ShaderSource);

            // Close file
            fclose(file);

            //set type
            Type = ShaderType;
            if(!glIsShader(ObjectID))
            {
                ObjectID = glCreateShader(Type);
            }

            return true;
        }

        /*** ShaderObject::ReloadShaderObjects
        * Attempt to reload the shader object. This will re-open the file, pull in the source, and attempt compilation
        * @return true if file the reload and compilation are both successful, false otherwise
        */
        bool ShaderObject::Reload()
        {
            LogInfo("reloading %s\n", Filename);

            if (!Load(Filename, Type))
            {
                return false;
            }

            if(!Compile())
            {
                return false;
            }

            for (ShaderProgram* Program : ProgramsIncludedIn)
            {
                Program->LinkShaderProgram();
            }

            return true;
        }
    }
}
