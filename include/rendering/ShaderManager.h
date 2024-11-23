//
// Created by Ryanc on 11/22/2024.
//

#pragma once
#include <memory>
#include <unordered_map>


class ShaderObject;
class ShaderProgram;

class ShaderManager
{
public:
    static ShaderManager& Get();

    std::shared_ptr<ShaderProgram> LoadShaderProgram();

private:
    ShaderManager() = default;
    ~ShaderManager() = default;

    std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> ShaderPrograms;
    std::unordered_map<std::string, std::unique_ptr<ShaderObject>> ShaderObjects;
};
