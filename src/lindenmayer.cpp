//
// Created by ryan on 5/31/24.
//

#include "lindenmayer.h"
#include <cstring>
#include "stdio.h"
#include "logging.hpp"

void LSystem::SetSeed(char* NewSeed)
{
    Seed = strdup(NewSeed);
}

void LSystem::SetAxiom(char* NewAxiom)
{
    Axiom = strdup(NewAxiom);
    LogDebug("Axiom=%s", NewAxiom);
}

void LSystem::AddRule(char c, const char *R)
{
    RewritingRules.push_back(LS_RewritingRule(c, R));
}

void LSystem::Rewrite(int Iterations)
{
    if(Axiom == nullptr)
    {
        return;
    }

    char* SourceString = GeneratedString != nullptr ? GeneratedString : Axiom;
    if(SourceString == nullptr)
    {
        return;
    }

    char WorkingBuffer[1024] = {0};
    for(int i = 0; i < 1024; i++)
    {
        fprintf(stdout, "%c", WorkingBuffer[i]);
    }
    fprintf(stdout, "\n");

    for(int i = 0; i < Iterations; i++)
    {
        if(GeneratedString != nullptr)
        {
            SourceString = GeneratedString;
        }

        size_t StrLength = strlen(SourceString);
        for(int c = 0; c < StrLength; c++)
        {

        }
    }
}

ColoredTriangleList* Turtle::DrawSystem(LSystem &System)
{
    char* SourceString = System.GeneratedString != nullptr ? System.GeneratedString : System.Axiom;
    if(SourceString == nullptr)
    {
        return nullptr;
    }
    size_t StrLength = strlen(SourceString);

    constexpr unsigned int MaxTriangles = 1024;
    ColoredTriangleList* Triangles = new ColoredTriangleList(1024);

    for(int i = 0; i < MaxTriangles; i++)
    {
        ColoredTriangle& Triangle = Triangles->TriData[i];

        glm::vec3 center;
        center.x = (rand() % 1000) / 1000.0f;
        center.y = (rand() % 1000) / 1000.0f;
        center.z = (rand() % 1000) / 1000.0f;

        center -= 0.5;
        center *= 10.0f;

        for(int v = 0; v < 3; v++)
        {
            glm::vec3& vertex = Triangles->TriData[i].VertexLocations[v];
            glm::vec3& color = Triangles->TriData[i].VertexColors[v];

            vertex.x = (rand() % 1000) / 1000.0;
            vertex.y = (rand() % 1000) / 1000.0;
            vertex.z = (rand() % 1000) / 1000.0;
            vertex *= 1.0f;

            vertex = center + vertex;

            color.x = (rand() % 1000) / 1000.0;
            color.y = (rand() % 1000) / 1000.0;
            color.z = (rand() % 1000) / 1000.0;

            LogDebug("\tvertex[%d]=(%2.2lf,%2.2lf,%2.2lf)\n", v, vertex.x, vertex.y, vertex.z );
            LogDebug("\tcolor[%d]=(%2.2lf,%2.2lf,%2.2lf)\n", v, color.x, color.y, color.z );
        }
    }

    return Triangles;


    for(int i = 0; i < StrLength; i++)
    {
        switch(SourceString[i])
        {
            case 'g':
                Location += (Direction * System.Distance);
            case 'f':
            case 'h':
            default:
                break;
        }
    }
}

