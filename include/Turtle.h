//
// Created by Ryanc on 10/20/2024.
//

#ifndef TURTLE_H
#define TURTLE_H

#include "Transform.h"
#include "ColoredTriangle.h"
#include "lindenmayer.h"


/* Turtle class
 * used for interpreting and drawing L-Systems
 */
class Turtle
{
public:
    Turtle()
            : MyTransform()
    //Location(glm::vec3(0)), Forwards(glm::vec3(0.0, 1.0, 0.0)), Right(glm::vec3(1.0, 0.0, 0.0))
    {

    }

    //Move forward a given distance
    void MoveForward(float Distance);

    //Turn Right a given angle
    void TurnRight(float Angle = 90.0f);
    void TurnLeft(float Angle = 90.0f);
    void PitchUp(float Angle = 90.0f);
    void PitchDown(float Angle = 90.0f);
    void RollLeft(float Angle = 90.0f);
    void RollRight(float Angle = 90.0f);

    //special movement
    void TurnAround();
    void RotateToVertical();

    //branch management
    void StartBranch();
    void CompleteBranch();
    void CutOffRemainderOfBranch();

    //move and draw commands
    void MoveForwardAndDraw();

    //polygon creation
    void StartPolygon();
    void RecordVertex();
    void DrawForwardNoRecord();
    void CompletePolygon();

    void IncorporatePredefinedSurface();
    void DecrementSegmentDiameter();
    void IncrementColorIndex();

    /** Turtle::DrawSystem
     * @param System - the system to draw
     * @return A list of ColoredTriangles
     */
    ColoredTriangleList* DrawSystem(LSystem& System);

    Transform MyTransform;
};


#endif //TURTLE_H
