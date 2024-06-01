//
// Created by ryan on 5/31/24.
//

#ifndef GLBP_LINDENMAYER_H
#define GLBP_LINDENMAYER_H

#include <vector>
#include <glm/glm.hpp>

struct LS_RewritingRule
{
    LS_RewritingRule(char c, const char* R);

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

class Turtle
{
public:
    float** DrawSystem(LSystem& System);

    glm::vec3 Location;
    glm::vec3 Direction;
};


#endif //GLBP_LINDENMAYER_H
