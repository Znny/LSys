//
// Created by ryan on 5/31/24.
//

#include "lindenmayer.h"
#include <cstring>
#include "stdio.h"

void LSystem::SetSeed(char* NewSeed)
{
    Seed = strdup(NewSeed);
}

void LSystem::SetAxiom(char* NewAxiom)
{
    Axiom = strdup(NewAxiom);
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

float** Turtle::DrawSystem(LSystem &System)
{
    char* SourceString = System.GeneratedString != nullptr ? System.GeneratedString : System.Axiom;
    if(SourceString == nullptr)
    {
        return nullptr;
    }
    size_t StrLength = strlen(SourceString);

    constexpr unsigned int MaxTriangles = 1024;
    float TempVertData[2][MaxTriangles*3] = {0};

    for(int i = 0; i < MaxTriangles*3; i+=3)
    {
        fprintf(stdout, "v[%d]=(%f, %f, %f)\n", TempVertData[0][i+0], TempVertData[0][i+1], TempVertData[0][i+2]);
        fprintf(stdout, "c[%d]=(%f, %f, %f)\n", TempVertData[1][i+0], TempVertData[1][i+1], TempVertData[1][i+2]);
    }

    for(int i = 0; i < StrLength; i++)
    {
        switch(SourceString[i])
        {
            case 'g':
                Location += (Direction * System.Distance);
            case 'f':
            case 'h':
        }
    }
}

