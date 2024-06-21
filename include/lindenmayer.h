//
// Created by ryan on 5/31/24.
//
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <cstring>

/** Lindenmayer System rewriting rule
 *  consists of a character and a string which said character is replaced with
 *  replacement occurs when a string is being 'rewritten'
 *      which replaces each character in the string occording to the rewriting rules
 */
struct LS_RewritingRule
{
    LS_RewritingRule() : LS_RewritingRule(' ', "")
    {

    }

    LS_RewritingRule(char In_CharacterReplaced, const char* In_ReplacementString)
    {
        CharacterReplaced = In_CharacterReplaced;
        ReplacementString = strdup(In_ReplacementString);
    }

    char CharacterReplaced;
    char* ReplacementString;

    void PasteReplacementString(char** LocationToPastePtr);
};

/* LSystem
 * A representation of a Lindenmayer System
 * Contains variables for rewriting, like the initial axiom, a number of iterations,
 *  and rewriting rules for each iteration
 * Also contains variables related to rendering the L-System using a turtle,
 *  such as the distance to move, or the angle to rotate by
 */
class LSystem
{

public:
    LSystem();

    /** AddRule
     * Adds a rewriting rule to the L-System, for when the given character is read, it is replaced with RewrittenString
     * @param character - the character to be rewritten
     * @param RewrittenString - the string the character should be replaced with
     */
    void AddRule(char character, const char* RewrittenString);

    /** SetAxiom
     * Sets the initial string, or starting point, of the L-System
     * @param NewAxiom - the new axiom string
     */
    void SetAxiom(char* NewAxiom);

    /** SetIterations
     * Sets the number of iterations to be run when Rewrite is called
     * @param iter - the new number of iterations
     *  creates new renderable for the given iteration, so it can be rendered
     */
    void SetIterations(int iter);

    /** LoadFromFile
     * Load L-System settings from a file

     * @param Filename
     */
    void LoadFromFile(const char* Filename);

    void Rewrite();


    //initial string which rewriting works off of
    char* Axiom;

    //the generated string from a number of rewritings, is used as an intermediary if multiple iterations occur
    char* GeneratedString;

    //rules for rewniting the axiom or generated string for each iteration of rewriting
    LS_RewritingRule RewritingRules[128];

    //the number of times the string should be rewritten, using the rewriting rules provided
    int Iterations;

    //the distance a turtle should move when a move command is read
    float Distance;

    //the angle a turtle should rotate when a rotation command is read
    float Angle;
};

/* ColoredTriangle
 * Three vertex locations for the triangle, and a color for each
 */
struct ColoredTriangle
{
    ColoredTriangle()
    {
        for (int i = 0; i < 3; i++)
        {
            VertexLocations[i] = glm::vec3(0);
            VertexColors[i] = glm::vec3(0);
        }
    }

    glm::vec3 VertexLocations[3]{};
    glm::vec3 VertexColors[3]{};
};

/* ColoredTriangleList
 * an array of colored triangles
 */
struct ColoredTriangleList
{
    ColoredTriangleList(int MaxTris)
    {
        NumTriangles = 0;
        MaxTriangles = MaxTris;
        TriData = (ColoredTriangle*) malloc(MaxTriangles * sizeof(ColoredTriangle));
    }

    ~ColoredTriangleList()
    {
        if (TriData != nullptr)
        {
            free(TriData);
        }
    }

    void AddTriangle(ColoredTriangle& Triangle)
    {
        TriData[NumTriangles++] = Triangle;
    }

    long long int NumTriangles = 0;
    long long int MaxTriangles = 0;
    ColoredTriangle* TriData = nullptr;
};


/* Turtle class
 * used for interpreting and drawing L-Systems
 */
class Turtle
{
public:
    Turtle()
            : Location(glm::vec3(0)), Forwards(glm::vec3(0.0, 1.0, 0.0)), Right(glm::vec3(1.0, 0.0, 0.0))
    {

    }

    /** Turtle::DrawSystem
     * @param System - the system to draw
     * @return A list of ColoredTriangles
     */
    ColoredTriangleList* DrawSystem(LSystem& System);

    glm::vec3 Location;
    glm::vec3 Forwards;
    glm::vec3 Right;
};

