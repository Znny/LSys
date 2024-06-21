//
// Created by Ryan on 6/21/2024.
//

#pragma once

#include <vector>

class ShaderProgram;
class ShaderObject;

class ZShaderSubsystem
{

public:
    static ZShaderSubsystem* ShaderSubsystem;

    /** LoadShadersDeferred
     * load all shaders which had been deferred for one reason or another
     */
    int LoadShadersDeferred();


private:

    //filenames or strings of deferred shaders
    std::vector<const char*> DeferredShaderPrograms;
    std::vector<const char*> DeferredShaderObjects;

    std::vector<ShaderProgram*> LoadedShaderPrograms;
    std::vector<ShaderObject*> LoadedShaderObjects;
};

//global variable for
extern ZShaderSubsystem ShaderSubsystem;


