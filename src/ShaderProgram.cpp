//
// Created by ryan on 5/26/24.
//

#include "ShaderProgram.h"
#include "logging.hpp"
#include <cstring>
#include <stdio.h>

namespace LSYS
{
    namespace Rendering
    {
        ShaderProgram::ShaderProgram(const char* name)
        {
            if (name != nullptr)
            {
                Name = strdup(name);
            }
            else
            {
                Name = strdup("unspecified");
            }

            ProgramID = glCreateProgram();
            LogVerbose("ShaderProgram allocated, ProgramID=%u\n", ProgramID);
        }

        void ShaderProgram::Attach(ShaderObject* Object)
        {
            if(Object == nullptr)
            {
                return;
            }

            LogInfo("attaching shader %s\n", Object->Filename);
            glAttachShader(ProgramID, Object->ObjectID);
            AttachedShaderObjects.push_back(Object);
            Object->ProgramsIncludedIn.push_back(this);
        }

        bool ShaderProgram::Link()
        {
            if (AttachedShaderObjects.size() < 2)
            {
                LogError("could not link shader, not enough shader objects attached\n");
                return false;
            }
            GLint linkStatus = GL_FALSE;
            int info_log_length = 0;

            LogInfo("linking \"%s\"...\n", Name);
            glLinkProgram(ProgramID);
            glGetProgramiv(ProgramID, GL_LINK_STATUS, &linkStatus);

            //log error if linking failed
            if (linkStatus != GL_TRUE)
            {
                GLsizei actualSize;
                glGetShaderiv(ProgramID, GL_INFO_LOG_LENGTH, &info_log_length);

                char info_log[1024];
                glGetProgramInfoLog(ProgramID, info_log_length, &actualSize, info_log);

                LogError("error:\n");
                LogError("%s\n", info_log);

                return false;
            }

            return true;
        }

        bool ShaderProgram::Compile()
        {
            bool bSucceeding = true;
            for (int i = 0; i < AttachedShaderObjects.size() && bSucceeding; i++)
            {
                bSucceeding |= AttachedShaderObjects[i]->Compile();
            }

            return bSucceeding;
        }

        void ShaderProgram::Reload()
        {
            bool bSucceeding = true;
            for (int i = 0; i < AttachedShaderObjects.size(); i++)
            {
                bSucceeding = bSucceeding && AttachedShaderObjects[i]->Reload();
            }
            if (bSucceeding)
            {
                if (Link())
                {
                    LogInfo("reloaded shader \"%s\"\n", Name);
                }
            }
        }

    }

}