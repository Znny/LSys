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
    //LogDebug("Axiom=%s", NewAxiom);
}

void LSystem::AddRule(char c, const char *R)
{
    LS_RewritingRule& Rule = RewritingRules[(int)c];
    Rule.Character = c;
    Rule.RString = strdup(R);
    LogDebug("Rule Added > %c:%s\n", Rule.Character, Rule.RString);
}

void LSystem::Rewrite()
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

    LogInfo("rewriting: %d iteration\n", Iterations);

    const int MaxCharacters = 1000000;
    char WorkingBuffer[MaxCharacters] = {0};
    LogInfo("\n");

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
            char& Character = SourceString[c];
            if(Character < 32)
            {
                continue;
            }
            LS_RewritingRule& Rule = RewritingRules[(int)Character];
            if(Rule.Character == Character && Rule.RString != nullptr)
            {
                //LogVerbose("using rule: %c:%s\n", Rule.Character, Rule.RString);
                strcat(WorkingBuffer, Rule.RString);
                NumGeneratedCharacters += strlen(Rule.RString);
            }
            else
            {
                WorkingBuffer[NumGeneratedCharacters++] = SourceString[c];
            }
            /*
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
             */
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

LSystem::LSystem()
{
    char* NewAxiom = strdup("F--F--F");
    SetAxiom(NewAxiom);
    Angle = glm::radians(60.0f);
    Distance = 0.2;
}

void LSystem::LoadFromFile(const char *Filename)
{
    if(Filename == nullptr)
    {
        return;
    }

    char line[1024];
    FILE* fp = fopen(Filename, "r");

    //Checks if file is empty
    if( fp == NULL )
    {
        return;
    }

    while( fgets(line,1024,fp) )
    {
        char* axiom = strstr(line, "x:");
        char* angle = strstr(line, "a:");
        char* iterations = strstr(line, "i:");

        if(axiom != nullptr)
        {
            SetAxiom(axiom+2);
        }
        else if (angle != nullptr)
        {
            Angle = glm::radians(atof(line+2));
        }
        else if (iterations != nullptr)
        {
            Iterations = atoi(line + 2);
        }
        else if(line[0] > 32 && line[1]==':')
        {
            AddRule(line[0], line+2);
        }
    }

    fclose(fp);
}

ColoredTriangleList* Turtle::DrawSystem(LSystem &System)
{
    char* SourceString = System.GeneratedString != nullptr ? System.GeneratedString : System.Axiom;
    if(SourceString == nullptr)
    {
        return nullptr;
    }
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

    glm::mat3 RollRight = glm::rotate(glm::identity<glm::mat4>(), System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat3 RollLeft = glm::rotate(glm::identity<glm::mat4>(), -System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat3 PitchUp = glm::rotate( glm::identity<glm::mat4>(), System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat3 PitchDown = glm::rotate( glm::identity<glm::mat4>(), -System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat3 YawLeft = glm::rotate( glm::identity<glm::mat4>(), System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat3 YawRight = glm::rotate( glm::identity<glm::mat4>(), -System.Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    //process string
    for(int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        ColoredTriangle Triangle;
        switch(SourceString[i])
        {
            case 'F':
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

