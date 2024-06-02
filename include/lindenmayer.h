//
// Created by ryan on 5/31/24.
//

#ifndef GLBP_LINDENMAYER_H
#define GLBP_LINDENMAYER_H

#include <vector>
#include <glm/glm.hpp>
#include <cstring>

struct LS_RewritingRule
{
    LS_RewritingRule(): LS_RewritingRule (' ', "")
    {

    }

    LS_RewritingRule(char c, const char* R)
    {
        Character = c;
        RString = strdup(R);
    }

    char Character;
    char* RString;
};

class LSystem
{

public:
    LSystem();

    void LoadFromFile(const char* Filename);

    void AddRule(char c, const char* R);
    void SetSeed(char* NewSeed);
    void SetAxiom(char* NewAxiom);
    void Rewrite();
    void SetIterations(int iter);

    //seed for PRNG
    char* Seed;

    //starting string
    char* Axiom;

    char* GeneratedString;

    //rewriting rules which effect the axiom
    LS_RewritingRule RewritingRules[128];

    float Distance;
    float Angle;
    int Iterations;
};

struct ColoredTriangle
{
    ColoredTriangle()
    {
        for(int i = 0; i < 3; i++)
        {
            VertexLocations[i] = glm::vec3(0);
            VertexColors[i] = glm::vec3(0);
        }
    }
    glm::vec3 VertexLocations[3];
    glm::vec3 VertexColors[3];
};

struct ColoredTriangleList
{
    ColoredTriangleList(int MaxTris)
    {
        NumTriangles = 0;
        MaxTriangles = MaxTris;
        TriData = (ColoredTriangle*) malloc( MaxTriangles * sizeof(ColoredTriangle));
    }

    ~ColoredTriangleList()
    {
        if(TriData != nullptr)
        {
            free(TriData);
        }
    }

    void AddTriangle(ColoredTriangle& Triangle)
    {
        TriData[NumTriangles++] = Triangle;
    }
    int NumTriangles;
    int MaxTriangles;
    ColoredTriangle* TriData;
};

class Turtle
{
public:
    Turtle()
    : Location(glm::vec3(0)),
    Forwards(glm::vec3(0.0, 1.0, 0.0)),
    Right(glm::vec3(1.0, 0.0, 0.0))
    {

    }

    ColoredTriangleList* DrawSystem(LSystem& System);

    glm::vec3 Location;
    glm::vec3 Forwards;
    glm::vec3 Right;
};


#endif //GLBP_LINDENMAYER_H
