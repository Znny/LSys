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

bool ShaderObject::Compile()
{
    LogInfo("compiling %s...\n", Filename ? Filename : "unknown");

    GLint compile_status = GL_FALSE;
    int info_log_length = 0;
    char info_log[1024];

    //set our source and attempt to compile
    glShaderSource( ObjectID, 1, (const GLchar**)(&ShaderSource), NULL );
    glCompileShader( ObjectID );
    glGetShaderiv( ObjectID, GL_COMPILE_STATUS, &compile_status );

    //check for compile errors
    if( compile_status != GL_TRUE )
    {
        glGetShaderiv( ObjectID, GL_INFO_LOG_LENGTH, &info_log_length );
        glGetShaderInfoLog( ObjectID, info_log_length, NULL, info_log );
        LogError("error: %s\n", info_log);
        return false;
    }

    return true;
}

bool ShaderObject::Load(const char* filename, GLenum ShaderType)
{
    if(filename == nullptr)
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
    ShaderSource = (char*)malloc(fileSize + 1);
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

    LogVerbose("Contents:\n%s\n", ShaderSource );

    // Close file
    fclose(file);

    //set type
    Type = ShaderType;
    ObjectID = glCreateShader(Type);

    return true;
}

ShaderObject::ShaderObject(const char *filename, GLenum shaderType)
{
    if(filename != nullptr)
    {
        Load(filename, shaderType);
    }
}

bool ShaderObject::Reload()
{
    LogInfo("reloading %s\n", Filename);

    if(Load(Filename, Type))
    {
        return Compile();
    }

    return false;
}
