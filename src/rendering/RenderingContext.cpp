//
// Created by Ryanc on 12/3/2024.
//

#include "rendering/RenderingContext.h"
#include "glad/glad.h"

void Rendering::RenderingContext::Initialize()
{
    glGenVertexArrays(1, &ActiveVAO);
    glBindVertexArray(ActiveVAO);

    glGenBuffers(1, &LocationVBO);
    glBindBuffer(GL_ARRAY_BUFFER, LocationVBO);
    //glBufferStorage(GL_ARRAY_BUFFER, sizeof(glm::vec3)*MaxVertices, nullptr, );

    glGenBuffers(1, &ColorVBO);
    glGenBuffers(1, &IndexBufferObject);

    glGenBuffers(1, &UBO_Block);
    glBindBuffer(GL_UNIFORM_BLOCK, UBO_Block);
    //glBufferStorage(GL_UNIFORM_BLOCK, sizeof(GlobalUniforms), nullptr)
}

void Rendering::RenderingContext::SetActiveCamera(Camera *Camera)
{

}

void Rendering::RenderingContext::Update()
{

}
