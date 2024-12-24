//
// Created by Ryanc on 10/20/2024.
//
#ifndef TURTLE_H
#define TURTLE_H

#include <vector>
#include "Transform.h"
#include "rendering/ColoredTriangle.h"
#include "lindenmayer/lindenmayer.h"
#include "Stack.h"

/* Turtle class
 * used for interpreting and drawing L-Systems
 */
class Turtle
{
public:
    Turtle()=default;

    void Reset();
    void MoveForward(float Distance);

    //special movement
    void TurnAround();
    void RotateToVertical();

    //branch management
    void StartBranch();
    void CompleteBranch();
    //void CutOffRemainderOfBranch();

    //move and draw commands
    //void MoveForwardAndDraw();

    //polygon creation
    void StartPolygon();
    //void RecordVertex();
    //void DrawForwardNoRecord();
    void CompletePolygon(ColoredTriangleList* triangles);

    //void IncorporatePredefinedSurface();
    //void DecrementSegmentDiameter();
    //void IncrementColorIndex();

    /** Turtle::DrawSystem
     * @param System - the system to draw
     * @return A list of ColoredTriangles
     */
    ColoredTriangleList* DrawSystem(LSystem& System);
    void DrawConeSegment(float r1, float r2, glm::vec3& color1, glm::vec3& color2, float length, ColoredTriangleList* triangles);

    //whether we are currently defining a polygon or not
    bool bIsDefiningPolygon = false;
    //transform when polygon was started via { character
    Transform PolygonStartTransform;
    //vertices in the current polygon
    std::vector<glm::vec3> polygonVertices;

    //current width being used when rendering conical sections
    float CurrentWidth = 1.0;
    //current transform of the turtle
    Transform CurrentTransform;
    //current color used when adding triangles
    glm::vec3 CurrentColor = glm::vec3(1.0, 0.0, 0.0);

    //state data for branches
    struct StateData
    {
        bool bIsDefiningPolygon = false;
        float CurrentWidth;
        Transform CurrentTransform;
        glm::vec3 CurrentColor;
    };
    Stack<StateData> BranchStack;
};


#endif //TURTLE_H
