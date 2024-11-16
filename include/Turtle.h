//
// Created by Ryanc on 10/20/2024.
//
#ifndef TURTLE_H
#define TURTLE_H

#include "Transform.h"
#include "ColoredTriangle.h"
#include "lindenmayer.h"
#include "Stack.h"

/* Turtle class
 * used for interpreting and drawing L-Systems
 */
class Turtle
{
public:
    Turtle()=default;

    void MoveForward(float Distance);

    //special movement
    //void TurnAround();
    //void RotateToVertical();

    //branch management
    void StartBranch();
    void CompleteBranch();
    //void CutOffRemainderOfBranch();

    //move and draw commands
    //void MoveForwardAndDraw();

    //polygon creation
    //void StartPolygon();
    //void RecordVertex();
    //void DrawForwardNoRecord();
    //void CompletePolygon();

    //void IncorporatePredefinedSurface();
    //void DecrementSegmentDiameter();
    //void IncrementColorIndex();

    /** Turtle::DrawSystem
     * @param System - the system to draw
     * @return A list of ColoredTriangles
     */
    ColoredTriangleList* DrawSystem(LSystem& System);

    Transform CurrentTransform;
    glm::vec3 CurrentColor = glm::vec3(1.0, 0.0, 0.0);

    struct StateData
    {
        Transform CurrentTransform;
        glm::vec3 CurrentColor;
    };
    Stack<StateData> BranchStack;
};


#endif //TURTLE_H
