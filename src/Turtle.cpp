//
// Created by Ryanc on 10/20/2024.
//

#include "Turtle.h"

void Turtle::MoveForward(float Distance)
{
    MyTransform.SetLocation(MyTransform.GetLocation() + MyTransform.GetForwardVector() * Distance);
}

void Turtle::YawRight(float Angle)
{
    MyTransform.Rotate(MyTransform.GetUpVector(), Angle);
}

void Turtle::YawLeft(float Angle)
{
    YawRight(-Angle);
}

void Turtle::PitchUp(float Angle)
{
    MyTransform.Rotate(MyTransform.GetRightVector(), Angle);
}

void Turtle::PitchDown(float Angle)
{
    PitchUp(-Angle);
}

void Turtle::RollLeft(float Angle)
{
    MyTransform.Rotate(MyTransform.GetForwardVector(), Angle);
}

void Turtle::RollRight(float Angle)
{
    RollLeft(-Angle);
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


    for(int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        switch (SourceString[i])
        {
            case 'F':
            {
                //cache half width
                glm::vec3 HalfWidth = MyTransform.GetRightVector() * 0.1f * System.Distance;

                //get start and end locations
                glm::vec3 StartLocation = MyTransform.GetLocation();
                MoveForward(System.Distance);
                glm::vec3 EndLocation = MyTransform.GetLocation();
                glm::vec3 TriangleVerts[4] =
                {
                    StartLocation - HalfWidth,
                    StartLocation + HalfWidth,
                    EndLocation - HalfWidth,
                    EndLocation + HalfWidth
                };

                Triangle.VertexLocations[0] = TriangleVerts[0];
                Triangle.VertexLocations[1] = TriangleVerts[1];
                Triangle.VertexLocations[2] = TriangleVerts[2];
                Triangle.RandomizeColors();
                Triangles->AddTriangle(Triangle);

                Triangle.VertexLocations[0] = TriangleVerts[2];
                Triangle.VertexLocations[1] = TriangleVerts[1];
                Triangle.VertexLocations[2] = TriangleVerts[3];
                Triangle.RandomizeColors();
                Triangles->AddTriangle(Triangle);
            }
            break;
            case 'f':
                MoveForward(System.Distance);
            break;
            case '+':
                YawRight(System.Angle);
            break;
            case '-':
                YawLeft(System.Angle);
            break;
            case '^':
                PitchUp(System.Angle);
            break;
            case '&':
                PitchDown(System.Angle);
            break;
            case '\\':
                RollLeft(System.Angle);
            break;
            case '/':
                RollRight(System.Angle);
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

