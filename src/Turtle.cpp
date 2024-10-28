//
// Created by Ryanc on 10/20/2024.
//

#include "Turtle.h"
#include "stdio.h"

void Turtle::MoveForward(float Distance)
{
    SetLocation(GetLocation() + GetForwardVector() * Distance);
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

    glm::vec3 CurrentColor;
    CurrentColor.r = (rand() % 1000) / 1000.0f;
    CurrentColor.g = (rand() % 1000) / 1000.0f;
    CurrentColor.g = 0.1;
    CurrentColor.b = (rand() % 1000) / 1000.0f;

    for(int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        switch (SourceString[i])
        {
            case 'F': {
                glm::vec3 NextColor;
                NextColor.r = (rand() % 1000) / 1000.0f;
                NextColor.g = (rand() % 1000) / 1000.0f;
                NextColor.g = 0.1;
                NextColor.b = (rand() % 1000) / 1000.0f;
                //cache half width
                glm::vec3 HalfWidth = GetRightVector() * 0.1f * System.Distance;

                //get start and end locations
                glm::vec3 StartLocation = GetLocation();
                MoveForward(System.Distance);
                glm::vec3 EndLocation = GetLocation();
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
                //Triangle.VertexLocations[0] = TriangleVerts[0];
                //Triangle.VertexLocations[1] = TriangleVerts[1];
                //Triangle.VertexLocations[2] = TriangleVerts[2];
                Triangles->AddTriangle(Triangle);

                Triangle.VertexLocations[0] = TriangleVerts[2];
                Triangle.VertexLocations[1] = TriangleVerts[1];
                Triangle.VertexLocations[2] = TriangleVerts[3];
                Triangle.VertexColors[0] = TriangleColors[2];
                Triangle.VertexColors[1] = TriangleColors[1];
                Triangle.VertexColors[2] = TriangleColors[3];
                //Triangle.RandomizeColors();
                Triangles->AddTriangle(Triangle);
                CurrentColor=NextColor;
            }
            break;
            case 'f':
                MoveForward(System.Distance);
            break;
            case '+':
                AdjustYaw(System.Angle);
            break;
            case '-':
                AdjustYaw(-System.Angle);
            break;
            case '^':
                AdjustPitch(System.Angle);
            break;
            case '&':
                AdjustPitch(-System.Angle);
            break;
            case '\\':
                AdjustRoll(System.Angle);
            break;
            case '/':
                AdjustRoll(-System.Angle);
            break;
        }
    }

    /*
    //process string
    for (int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        glm::vec3 StartLocation = MyTransform.GetTranslation();
        ColoredTriangle Triangle;
        switch (SourceString[i])
        {
            case 'F':
            {
                glm::vec3 EndLocation = StartLocation + MyTransform.GetForwardVector() * System.Distance;
                glm::vec3 HalfWidth = MyTransform.GetRightVector() * 0.1f * System.Distance;
                Triangle.VertexLocations[0] = StartLocation + HalfWidth;
                Triangle.VertexLocations[1] = StartLocation - HalfWidth;
                Triangle.VertexLocations[2] = EndLocation + HalfWidth;
                for (int ci = 0; ci < 3; ci++)
                {
                    Triangle.VertexColors[ci].r = (rand() % 1000) / 1000.0f;
                    Triangle.VertexColors[ci].g = (rand() % 1000) / 1000.0f;
                    Triangle.VertexColors[ci].b = (rand() % 1000) / 1000.0f;
                }
                Triangles->AddTriangle(Triangle);
                Triangle.VertexLocations[0] = StartLocation - HalfWidth;
                Triangle.VertexLocations[1] = EndLocation + HalfWidth;
                Triangle.VertexLocations[2] = EndLocation - HalfWidth;
                Triangles->AddTriangle(Triangle);
            }
            case 'f':
                MyTransform.SetLocation(StartLocation + MyTransform.GetForwardVector() * System.Distance);
                //Location += (Forwards * System.Distance);
                break;
            case '+':
                MyTransform.Rotate(MyTransform.GetRightVector(), 90.0f);
                break;
            case '-':
                MyTransform.Rotate(MyTransform.GetRightVector(), -90.0f);
                break;
            default:
                break;
        }
    }
    */

    return Triangles;
}

