//
// Created by Ryanc on 10/20/2024.
//

#include "utility/Turtle.h"
#include <stdio.h>
#include <cstring>
#include <utility/logging.hpp>
#include <utility/logging.inl>

void Turtle::Reset()
{
    CurrentTransform.Reset();
}

void Turtle::MoveForward(float Distance)
{
    CurrentTransform.SetLocation(CurrentTransform.GetLocation() + CurrentTransform.GetForwardVector() * Distance);
}

ColoredTriangleList* Turtle::DrawSystem(LSystem& System)
{
    CurrentWidth = System.Distance / 3.141592f;
    float WidthDecrement = CurrentWidth / (3.141592f * 3.141592f * 3.141592f);

    char* SourceString = System.GeneratedString != nullptr
                         ? System.GeneratedString
                         : System.Axiom;
    if (SourceString == nullptr)
    {
        return nullptr;
    }
    size_t StrLength = strlen(SourceString);

    constexpr unsigned int MaxTriangles = 1000000;
    ColoredTriangleList* Triangles = new ColoredTriangleList(MaxTriangles);

    ColoredTriangle Triangle;

    CurrentColor.r = (rand() % 1000) / 1000.0f;
    CurrentColor.g = (rand() % 1000) / 1000.0f;
    CurrentColor.g = 0.1;
    CurrentColor.b = (rand() % 1000) / 1000.0f;

    LogVerbose("Turtle Processing string of length %d\n", StrLength);

    for(int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        switch (SourceString[i])
        {
            case 'F':
            {
                glm::vec3 NextColor;
                NextColor.r = (rand() % 1000) / 1000.0f;
                NextColor.g = (rand() % 1000) / 1000.0f;
                NextColor.g = 0.1;
                NextColor.b = (rand() % 1000) / 1000.0f;

                DrawConeSegment(CurrentWidth, CurrentWidth-WidthDecrement, CurrentColor, NextColor, System.Distance, Triangles);
                CurrentWidth -= WidthDecrement;
                CurrentColor = NextColor;
                MoveForward(System.Distance);
            }
            break;

            case 'f':
                MoveForward(System.Distance);
            break;

            //yaw left right by system angle
            case '+':
                CurrentTransform.AdjustYaw(System.Angle);
            break;
            case '-':
                CurrentTransform.AdjustYaw(-System.Angle);
            break;

            //Pitch up/down by system angle
            case '^':
                CurrentTransform.AdjustPitch(System.Angle);
            break;
            case '&':
                CurrentTransform.AdjustPitch(-System.Angle);
            break;

            //Roll right/left by system angle
            case '\\':
                CurrentTransform.AdjustRoll(System.Angle);
            break;
            case '/':
                CurrentTransform.AdjustRoll(-System.Angle);
            break;

            //Turn Around
            case '|':
                CurrentTransform.AdjustYaw(180);
            break;

            case '$':
            {
                //todo: implement rotate turtle to vertical (pg 57)
                //rolls the turtle around its own axis so that vector L-> pointing to the left of the turtle is brought to a horizoental position
                //consequently, the branch plane is 'closest tot he horizontal plane'.
                // as required by Honda's model. From a technical point of view, $ modifies the turtle orientation in space according to the formulae
                /* -->    -->   -->
                 *  L   =  V  x  H
                 *       -----------
                 *       |-->   -->|
                 *       | V  x  H |
                 *
                 * H = Heading  L = Left  U = Up
                 * V is the direction opposite to gravity, and |A| denotes the length of vector A
                 *
                 * L = V cross H / length(V cross H)
                 */

                glm::vec3 NewLeftDir = glm::cross(Transform::WorldUp, CurrentTransform.GetForwardVector());
                NewLeftDir = NewLeftDir / glm::length(NewLeftDir);

                glm::vec3 NewUpDir = glm::cross(CurrentTransform.GetForwardVector(), NewLeftDir);
                glm::normalize(NewUpDir);

                glm::mat3 RotationMatrix(CurrentTransform.GetForwardVector(), NewUpDir, NewLeftDir);
                CurrentTransform.SetRotation(RotationMatrix);
            }
            break;

            //start branch
            case '[':
                BranchStack.Push({CurrentWidth, CurrentTransform, CurrentColor});
            break;

            //stop branch
            case ']':
            {
                StateData Data = BranchStack.Pop();
                CurrentWidth = Data.CurrentWidth;
                CurrentTransform = Data.CurrentTransform;
                CurrentColor = Data.CurrentColor;
            }
            break;

            //begin polygon
            case '{':
                //todo: implement start a polygon
            break;

            case 'G':
                //implement Move forward and draw a line. do not record a vertex, pg 122
            break;

            case '.':
                //todo: implement record a vertex in the current polygon (pg 122, 127)
            break;

            case '}':
                //todo: implement complete a polygon
            break;

            case '~':
                //todo: implement incorporation of predefined surface
            break;

            case '!':
                //todo: implement decrement diamater of segments
            break;

            case '`':
                //todo: implement increment current color index
            break;

            case '%':
                //todo: implement cut off remainder of branch
            break;
        }
    }

    return Triangles;
}

void Turtle::DrawConeSegment(float r1, float r2, glm::vec3& color1, glm::vec3& color2, float length, ColoredTriangleList* triangles)
{
    // Define vertices for the cone segment
    const glm::vec3 start = CurrentTransform.GetLocation();
    const glm::vec3 end = start + CurrentTransform.GetForwardVector() * length;

    constexpr int numSides = 7; // Number of sides for the cone
    glm::vec3 circleStart[numSides];
    glm::vec3 circleEnd[numSides];

    // Generate circular cross sections
    for (int i = 0; i < numSides; ++i)
    {
        float angle = (2.0f * M_PI * i) / numSides;
        float x = cos(angle);
        float z = sin(angle);
        circleStart[i] = start + CurrentTransform.GetRightVector() * (r1 * x)
                               +CurrentTransform.GetUpVector() * (r1 * z);
        circleEnd[i] = end + CurrentTransform.GetRightVector() * (r2 * x)
                           + CurrentTransform.GetUpVector() * (r2 * z);
    }

    // Generate triangles for the cone
    for (int CurrentIndex = 0; CurrentIndex < numSides; ++CurrentIndex)
    {
        int NextIndex = (CurrentIndex + 1) % numSides;

        ColoredTriangle t1;
        t1.VertexLocations[0] = circleStart[CurrentIndex];
        t1.VertexLocations[1] = circleStart[NextIndex];
        t1.VertexLocations[2] = circleEnd[CurrentIndex];
        t1.VertexColors[0] = color1;
        t1.VertexColors[1] = color1;
        t1.VertexColors[2] = color2;

        ColoredTriangle t2;
        t2.VertexLocations[0] = circleEnd[NextIndex];
        t2.VertexLocations[1] = circleEnd[CurrentIndex];
        t2.VertexLocations[2] = circleStart[NextIndex];
        t2.VertexColors[0] = color2;
        t2.VertexColors[1] = color2;
        t2.VertexColors[2] = color1;

        triangles->AddTriangle(t1);
        triangles->AddTriangle(t2);
    }
}

