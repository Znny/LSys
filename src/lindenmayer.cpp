//
// Created by ryan on 5/31/24.
//

#include "lindenmayer.h"
#include <cstring>
#include "stdio.h"
#include "logging.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

    fprintf(stdout, "rewriting: %d iteration\n", Iterations);

    const int MaxCharacters = 1000000;
    char WorkingBuffer[MaxCharacters] = {0};
    fprintf(stdout, "\n");

    for(int i = 0; i < Iterations; i++)
    {
        if(GeneratedString != nullptr)
        {
            SourceString = GeneratedString;
        }

        size_t StrLength = strlen(SourceString);
        int NumGeneratedCharacters = 0;
        for(int c = 0; c < StrLength; c++)
        {
            switch(SourceString[c])
            {
                case 'f':
                    strcat(WorkingBuffer, "f+f--f+f");
                    NumGeneratedCharacters += 8;
                    break;
                default:
                    WorkingBuffer[NumGeneratedCharacters++] = SourceString[c];
                break;
            }
        }

        //if the previously generated string is not null, free it
        if(GeneratedString != nullptr)
        {
            free(GeneratedString);
        }

        //set the newly generated string to the previous contents of the working buffer
        GeneratedString = strdup(WorkingBuffer);

        //clear the working buffer
        for(char& c : WorkingBuffer)
        {
            c = 0;
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
    fprintf(stdout, "drawing string: %s\n", SourceString);
    size_t StrLength = strlen(SourceString);

    constexpr unsigned int MaxTriangles = 1000000;
    ColoredTriangleList* Triangles = new ColoredTriangleList(1000000);

    constexpr bool bGenTriangleSoup = false;
    if(bGenTriangleSoup)
    {
        for (int i = 0; i < MaxTriangles; i++) {
            ColoredTriangle &Triangle = Triangles->TriData[i];

            //set center to -5 to 5
            glm::vec3 center;
            center.x = (rand() % 1000) / 1000.0f;
            center.y = (rand() % 1000) / 1000.0f;
            center.z = (rand() % 1000) / 1000.0f;
            center -= 0.5;
            center *= 10.0f;

            for (int v = 0; v < 3; v++) {
                glm::vec3 &vertex = Triangles->TriData[i].VertexLocations[v];
                glm::vec3 &color = Triangles->TriData[i].VertexColors[v];

                vertex.x = (rand() % 1000) / 1000.0;
                vertex.y = (rand() % 1000) / 1000.0;
                vertex.z = (rand() % 1000) / 1000.0;
                vertex *= 1.0f;

                vertex = center + vertex;

                color.x = (rand() % 1000) / 1000.0;
                color.y = (rand() % 1000) / 1000.0;
                color.z = (rand() % 1000) / 1000.0;

                LogDebug("\tvertex[%d]=(%2.2lf,%2.2lf,%2.2lf)\n", v, vertex.x, vertex.y, vertex.z);
                LogDebug("\tcolor[%d]=(%2.2lf,%2.2lf,%2.2lf)\n", v, color.x, color.y, color.z);
            }
        }
    }

    glm::mat3 RRotate = glm::rotate( glm::identity<glm::mat4>(), System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat3 LRotate = glm::rotate( glm::identity<glm::mat4>(), -System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    //process string
    for(int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        ColoredTriangle Triangle;
        switch(SourceString[i])
        {
            case 'f':
            case 'h':
            {
                glm::vec3 StartLocation = Location;
                glm::vec3 EndLocation = Location + Forwards * System.Distance;
                glm::vec3 HalfWidth = Right * 0.1f * System.Distance;
                Triangle.VertexLocations[0] = StartLocation + HalfWidth;
                Triangle.VertexLocations[1] = StartLocation - HalfWidth;
                Triangle.VertexLocations[2] = EndLocation + HalfWidth;
                for(int ci = 0; ci < 3; ci++)
                {
                    Triangle.VertexColors[ci].r = (rand() % 1000) / 1000.0f;
                    Triangle.VertexColors[ci].g = (rand() % 1000) / 1000.0f;
                    Triangle.VertexColors[ci].b = (rand() % 1000) / 1000.0f;
                }
                Triangles->AddTriangle(Triangle);
                Triangle.VertexLocations[0] = StartLocation - HalfWidth;
                Triangle.VertexLocations[1] = EndLocation + HalfWidth;
                Triangle.VertexLocations[2] = EndLocation - HalfWidth;
                Triangles->AddTriangle(Triangle);
            }
            case 'g':
                Location += (Forwards * System.Distance);
                break;
            case '+':
                Forwards = Forwards * RRotate;
                Right = Right * RRotate;
                break;
            case '-':
                Forwards = Forwards * LRotate;
                Right = Right * LRotate;
                break;
            default:
                break;
        }
    }

    return Triangles;
}

