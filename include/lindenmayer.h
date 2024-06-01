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
    void AddRule(char c, const char* R);
    void SetSeed(char* NewSeed);
    void SetAxiom(char* NewAxiom);
    void Rewrite(int Iterations);

    //seed for PRNG
    char* Seed;

    //starting string
    char* Axiom;

    char* GeneratedString;

    //rewriting rules which effect the axiom
    std::vector<LS_RewritingRule> RewritingRules;

    float Distance;
    float Angle;
};

struct ColoredTriangle
{
    glm::vec3 VertexLocations[3];
    glm::vec3 VertexColors[3];
};

struct ColoredTriangleList
{
    ColoredTriangleList(int MaxTris)
    {
        NumTriangles = MaxTris;
        TriData = (ColoredTriangle*) malloc( MaxTris * sizeof(ColoredTriangle));
    }
    ~ColoredTriangleList()
    {
        if(TriData != nullptr)
        {
            free(TriData);
        }
    }
    int NumTriangles;
    ColoredTriangle* TriData;
};

class Turtle
{
public:
    ColoredTriangleList* DrawSystem(LSystem& System);

    glm::vec3 Location;
    glm::vec3 Direction;
};


#endif //GLBP_LINDENMAYER_H
