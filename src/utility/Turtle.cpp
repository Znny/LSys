//
// Created by Ryanc on 10/20/2024.
//

#include "utility/Turtle.h"
#include <stdio.h>
#include <cstring>

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
                //cache half width
                glm::vec3 HalfWidth = CurrentTransform.GetRightVector() * 0.1f * System.Distance;

                //get start and end locations
                glm::vec3 StartLocation = CurrentTransform.GetLocation();
                MoveForward(System.Distance);
                glm::vec3 EndLocation = CurrentTransform.GetLocation();
                glm::vec3 TriangleVerts[4] =
                {
                    StartLocation - HalfWidth,
                    StartLocation + HalfWidth,
                    EndLocation - HalfWidth,
                    EndLocation + HalfWidth
                };
                glm::vec3 TriangleColors[4] =
                {
                    CurrentColor, CurrentColor,
                    NextColor, NextColor
                };

                for(int j = 0; j < 3; j++)
                    {
                    Triangle.VertexLocations[j] = TriangleVerts[j];
                    Triangle.VertexColors[j] = TriangleColors[j];
                }
                Triangles->AddTriangle(Triangle);

                Triangle.VertexLocations[0] = TriangleVerts[2];
                Triangle.VertexLocations[1] = TriangleVerts[1];
                Triangle.VertexLocations[2] = TriangleVerts[3];
                Triangle.VertexColors[0] = TriangleColors[2];
                Triangle.VertexColors[1] = TriangleColors[1];
                Triangle.VertexColors[2] = TriangleColors[3];
                Triangles->AddTriangle(Triangle);
                CurrentColor=NextColor;
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
                BranchStack.Push({CurrentTransform, CurrentColor});
            break;

            //stop branch
            case ']':
            {
                StateData Data = BranchStack.Pop();
                CurrentTransform = Data.CurrentTransform;
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

