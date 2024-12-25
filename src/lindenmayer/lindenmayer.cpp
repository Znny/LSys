//
// Created by ryan on 5/31/24.
//

#include "lindenmayer/lindenmayer.h"
#include <cstring>
#include <cstdio>
#include "utility/logging.hpp"
#include "glm/gtc/matrix_transform.hpp"

LS_RewritingRule::LS_RewritingRule(const char c, const char *R)
{
    Character = c;
    strncat(RString, R, strnlen(R, MaxReplacementLength));
}

/** LSystem::LSystem
 * Default constructor for L-Systems
 */
LSystem::LSystem()
{
    SetName("Default");
    SetAxiom("F--F--F");
    Angle = 60.0f;
    Distance = 0.2;
}

void LSystem::SetName(const char* NewName)
{
    Name = strdup(NewName);
}

/** LSystem::SetAxiom
 *
 * @param NewAxiom
 */
void LSystem::SetAxiom(const char* NewAxiom)
{
    Axiom = strdup(NewAxiom);
}

void LSystem::SetIterations(int NewIterations)
{
    Iterations = NewIterations;
}

/** LSystem::AddRule
 *
 * @param character
 * @param RewrittenString
 */
void LSystem::AddRule(const char character, const char* RewrittenString)
{
    LS_RewritingRule& Rule = RewritingRules[static_cast<unsigned char>(character)];
    Rule.Character = character;
    strncat(Rule.RString, RewrittenString, strnlen(RewrittenString, MaxReplacementLength));
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

    LogInfo("rewriting %d times...\n", Iterations);

    constexpr int MaxCharacters = 1000000;
    char WorkingBuffer[MaxCharacters] = {0};
    LogInfo("\n");

    for (int i = 0; i < Iterations; i++)
    {
        if (GeneratedString != nullptr)
        {
            SourceString = GeneratedString;
        }

        size_t StrLength = strlen(SourceString);
        size_t NumGeneratedCharacters = 0;
        for (int c = 0; c < StrLength; c++)
        {
            char& Character = SourceString[c];
            if (Character < 32)
            {
                continue;
            }

            const LS_RewritingRule& Rule = RewritingRules[static_cast<unsigned char>(Character)];

            const bool bUsingExplicitRule = Rule.Character == Character && Rule.RString != nullptr;

            //if new length would exceed max characters, exit early
            const size_t AddedLength = bUsingExplicitRule ? strlen(Rule.RString) : 1;
            if(NumGeneratedCharacters + AddedLength >= MaxCharacters)
            {
                LogWarning("Num generated characters exceeds 10mil limit, stopping...\n");
                break;
            }


            //concatenate characters
            if(bUsingExplicitRule)
            {
                strcat(WorkingBuffer, Rule.RString);
            }
            else
            {
                WorkingBuffer[NumGeneratedCharacters] = SourceString[c];
            }

            //set new length
            NumGeneratedCharacters += AddedLength;
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

        LogInfo("rewrite %d complete...\n", i);
    }

    LogInfo("Rewriting complete\n");
}

void LSystem::Reset()
{
    if(GeneratedString != nullptr)
    {
        free(GeneratedString);
        GeneratedString = nullptr;
    }
}

void LSystem::SetAngle(float NewAngle)
{
    Angle = NewAngle;
}

void LSystem::SetDistance(float NewDistance)
{
    Distance = NewDistance;
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
    if (fp == nullptr)
    {
        return;
    }

    while (fgets(line, 1024, fp))
    {
        const char* AxiomString = "axiom:";
        const char* AngleString = "angle:";
        const char* IterationsString = "iterations:";

        const char* axiomStart = strstr(line, AxiomString);
        const char* angleStart = strstr(line, AngleString);
        const char* iterationsStart = strstr(line, IterationsString);

        if (axiomStart != nullptr)
        {
            SetAxiom(axiomStart + strlen(AxiomString));
        }
        else if (angleStart != nullptr)
        {
            Angle = static_cast<float>(strtod(angleStart + strlen(AngleString), nullptr));
        }
        else if (iterationsStart != nullptr)
        {
            Iterations = strtol(iterationsStart + strlen(IterationsString), nullptr, 10);
        }
        else if (line[0] > 32 && line[1] == ':')
        {
            AddRule(line[0], line + 2);
        }
    }

    fclose(fp);
}

void LSystem::AddRuleFromString(const char* String)
{
    if(String == nullptr)
    {
        return;
    }
    const int ColonOffset = static_cast<int>(strstr(String, ":") - String);
    if(ColonOffset < 0)
    {
        return;
    }

    char* FullString = strdup(String);
    FullString[ColonOffset] = '\0';
    AddRule(*FullString, FullString+ColonOffset+1);
}

void LSystem::SaveToFile(const char* Filename)
{
    if (Filename == nullptr)
    {
        return;
    }

    //attempt to open the file
    FILE* fp = fopen(Filename, "w");

    //ensure the file was opened
    if (fp == nullptr)
    {
        return;
    }

    //write name
    if(Name != nullptr)
    {
        fprintf(fp, "name:%s\n", Name);
    }

    //write axiom
    if(Axiom != nullptr)
    {
        fprintf(fp, "axiom:%s\n", Axiom);
    }

    //write angle
    fprintf(fp, "angle:%f\n", Angle);

    //write iterations
    fprintf(fp, "iterations:%d\n", Iterations);

    //write rules
    for(LS_RewritingRule& Rule : RewritingRules)
    {
        if(Rule.Character != ' ')
        {
            fprintf(fp, "%c:%s\n", Rule.Character, Rule.RString);
        }
    }

    fclose(fp);
}

