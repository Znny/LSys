//
// Created by ryan on 5/31/24.
//

#include "lindenmayer.h"
#include <cstring>
#include "stdio.h"
#include "logging.hpp"
#include "glm/gtc/matrix_transform.hpp"

/** LSystem::LSystem
 * Default constructor for L-Systems
 */
LSystem::LSystem()
{
    char* NewAxiom = strdup("F--F--F");
    SetAxiom(NewAxiom);
    Angle = glm::radians(60.0f);
    Distance = 0.2;
}

/** LSystem::SetAxiom
 *
 * @param NewAxiom
 */
void LSystem::SetAxiom(char* NewAxiom)
{
    Axiom = strdup(NewAxiom);
}

/** LSystem::AddRule
 *
 * @param c
 * @param R
 */
void LSystem::AddRule(char c, const char* R)
{
    LS_RewritingRule& Rule = RewritingRules[(int) c];
    Rule.Character = c;
    Rule.RString = strdup(R);
}

/** LSystem::Rewrite
 *
 */
void LSystem::Rewrite()
{
    if (Axiom == nullptr)
    {
        return;
    }

    char* SourceString = GeneratedString != nullptr
                         ? GeneratedString
                         : Axiom;
    if (SourceString == nullptr)
    {
        return;
    }

    LogInfo("rewriting: %d iteration\n", Iterations);

    const int MaxCharacters = 1000000;
    char WorkingBuffer[MaxCharacters] = {0};
    LogInfo("\n");

    for (int i = 0; i < Iterations; i++)
    {
        if (GeneratedString != nullptr)
        {
            SourceString = GeneratedString;
        }

        size_t StrLength = strlen(SourceString);
        int NumGeneratedCharacters = 0;
        for (int c = 0; c < StrLength; c++)
        {
            char& Character = SourceString[c];
            if (Character < 32)
            {
                continue;
            }
            LS_RewritingRule& Rule = RewritingRules[(int) Character];
            if (Rule.Character == Character && Rule.RString != nullptr)
            {
                strcat(WorkingBuffer, Rule.RString);
                NumGeneratedCharacters += strlen(Rule.RString);
            }
            else
            {
                WorkingBuffer[NumGeneratedCharacters++] = SourceString[c];
            }
        }

        //if the previously generated string is not null, free it
        if (GeneratedString != nullptr)
        {
            free(GeneratedString);
        }

        //set the newly generated string to the previous contents of the working buffer
        GeneratedString = strdup(WorkingBuffer);

        //clear the working buffer
        for (char& c : WorkingBuffer)
        {
            c = 0;
        }
    }
}



/** LSystem::LoadFromFile
 *
 * @param Filename
 */
void LSystem::LoadFromFile(const char* Filename)
{
    if (Filename == nullptr)
    {
        return;
    }

    char line[1024];
    FILE* fp = fopen(Filename, "r");

    //Checks if file is empty
    if (fp == NULL)
    {
        return;
    }

    while (fgets(line, 1024, fp))
    {
        char* axiom = strstr(line, "x:");
        char* angle = strstr(line, "a:");
        char* iterations = strstr(line, "i:");

        if (axiom != nullptr)
        {
            SetAxiom(axiom + 2);
        }
        else if (angle != nullptr)
        {
            Angle = glm::radians(atof(line + 2));
        }
        else if (iterations != nullptr)
        {
            Iterations = atoi(line + 2);
        }
        else if (line[0] > 32 && line[1] == ':')
        {
            AddRule(line[0], line + 2);
        }
    }

    fclose(fp);
}

/** Turtle::DrawSystem
 *
 * @param System
 * @return
 */
ColoredTriangleList* Turtle::DrawSystem(LSystem& System)
{
    char* SourceString = System.GeneratedString != nullptr
                         ? System.GeneratedString
                         : System.Axiom;
    if (SourceString == nullptr)
    {
        return nullptr;
    }
    size_t StrLength = strlen(SourceString);

    constexpr unsigned int MaxTriangles = 1000000;
    ColoredTriangleList* Triangles = new ColoredTriangleList(MaxTriangles);

    glm::mat3 RollRight = glm::rotate(glm::identity<glm::mat4>(), System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat3 RollLeft = glm::rotate(glm::identity<glm::mat4>(), -System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat3 PitchUp = glm::rotate(glm::identity<glm::mat4>(), System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat3 PitchDown = glm::rotate(glm::identity<glm::mat4>(), -System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat3 YawLeft = glm::rotate(glm::identity<glm::mat4>(), System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat3 YawRight = glm::rotate(glm::identity<glm::mat4>(), -System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    //process string
    for (int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        ColoredTriangle Triangle;
        switch (SourceString[i])
        {
            case 'F':
            {
                glm::vec3 StartLocation = Location;
                glm::vec3 EndLocation = Location + Forwards * System.Distance;
                glm::vec3 HalfWidth = Right * 0.1f * System.Distance;
                Triangle.VertexLocations[0] = StartLocation + HalfWidth;
                Triangle.VertexLocations[1] = StartLocation - HalfWidth;
                Triangle.VertexLocations[2] = EndLocation + HalfWidth;
                for (int ci = 0; ci < 3; ci++)
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
            case 'f':
                Location += (Forwards * System.Distance);
                break;
            case '+':
                Forwards = Forwards * RollRight;
                Right = Right * RollRight;
                break;
            case '-':
                Forwards = Forwards * RollLeft;
                Right = Right * RollLeft;
                break;
            default:
                break;
        }
    }

    return Triangles;
}

