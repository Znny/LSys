//
// Created by Ryanc on 10/20/2024.
//

#include "utility/Turtle.h"
#include <cstring>
#include <utility/logging.hpp>
#include <utility/logging.inl>
#include <utility/util.h>

void Turtle::Reset()
{
    CurrentTransform.Reset();
}

void Turtle::MoveForward(float Distance)
{
    if(bIsDefiningPolygon)
    {
        CurrentTransform.SetLocation(CurrentTransform.GetLocation() + CurrentTransform.GetForwardVector() * (Distance-CurrentWidth));
        polygonVertices.push_back(CurrentTransform.GetLocation());
    }
    else
    {
        CurrentTransform.SetLocation(CurrentTransform.GetLocation() + CurrentTransform.GetForwardVector() * Distance);
    }
}

void Turtle::DrawSystem(LSystem& System, ColoredTriangleList** List)
{
    //set starting HSV color, at set to current color
    glm::vec3 CurrentHSVColor = {26.3, 0.7, .315};
    CurrentColor = HSVtoRGB(CurrentHSVColor);

    //set current width and how much to decrement when extending via 'F'
    CurrentWidth = System.Distance / 3.141592f;
    const float WidthDecrement = CurrentWidth / (3.141592f * 3.141592f * 3.141592f);

    //set source string based on whether we're working off a generated string or the axiom
    const char* SourceString = System.GeneratedString != nullptr
                                   ? System.GeneratedString
                                   : System.Axiom;

    //exit early if the source string is nullptr for some reason
    if (SourceString == nullptr)
    {
        return;
    }
    const size_t StrLength = strlen(SourceString);

    //specify max number of tris (10mil) and create list to store them all
    constexpr unsigned int MaxTriangles = 10000000;
    //auto* Triangles = new ColoredTriangleList(MaxTriangles);
    if(*List == nullptr)
    {
        *List = new ColoredTriangleList(MaxTriangles);
    }
    else {
        (*List)->Clear();
    }
    auto* Triangles = *List;

    LogVerbose("Turtle Processing string of length %d\n", StrLength);

    //iterate over the string, processing symbols as we go
    for(int i = 0; i < StrLength && Triangles->NumTriangles < MaxTriangles; i++)
    {
        switch (SourceString[i])
        {
            case 'F':
            {
                CurrentHSVColor = RGBtoHSV(CurrentColor);
                CurrentHSVColor.r = static_cast<float>(fmod(CurrentHSVColor.r + 8.0, 360.0));

                glm::vec3 NextColor = HSVtoRGB(CurrentHSVColor);

                DrawConeSegment(CurrentWidth, CurrentWidth-WidthDecrement, CurrentColor, NextColor, System.Distance, Triangles);
                CurrentWidth -= WidthDecrement;
                CurrentWidth = CurrentWidth <= 0.005f ? 0.005f : CurrentWidth;
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
                TurnAround();
            break;

            case '$':
            {
                RotateToVertical();
            }
            break;

            //start branch
            case '[':
                StartBranch();
            break;

            //stop branch
            case ']':
            {
                CompleteBranch();
            }
            break;

            //begin polygon
            case '{':
            {
                StartPolygon();
            }
            break;

            case 'G':
                //implement Move forward and draw a line. do not record a vertex, pg 122
            break;

            case '.':
                //todo: implement record a vertex in the current polygon (pg 122, 127)
            break;

            case '}':
            {
                CompletePolygon(Triangles);
            }
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
            default:
            break;
        }
    }
}

void Turtle::DrawConeSegment(float r1, float r2, glm::vec3& color1, glm::vec3& color2, float length, ColoredTriangleList* triangles) const {
    // Define vertices for the cone segment
    const glm::vec3 start = CurrentTransform.GetLocation();
    const glm::vec3 end = start + CurrentTransform.GetForwardVector() * length;

    constexpr int numSides = 11; // Number of sides for the cone
    glm::vec3 circleStart[numSides];
    glm::vec3 circleEnd[numSides];

    // Generate circular cross sections
    for (int i = 0; i < numSides; ++i)
    {
        double angle = (2.0f * M_PI * i) / numSides;
        double angle2= angle + (2.0f * M_PI * 0.5) / numSides;
        double x = cos(angle);
        double z = sin(angle);
        double x2 = cos(angle2);
        double z2 = sin(angle2);
        auto r1x = static_cast<float>(r1 * x);
        auto r1z = static_cast<float>(r1 * z);
        auto r2x2 = static_cast<float>(r2 * x2);
        auto r2z2 = static_cast<float>(r2 * z2);
        circleStart[i] = start + CurrentTransform.GetRightVector() * r1x
                               +CurrentTransform.GetUpVector() * r1z;
        circleEnd[i] = end + CurrentTransform.GetRightVector() * r2x2
                           + CurrentTransform.GetUpVector() * r2z2;
    }

    // Generate triangles for the cone
    for (int CurrentIndex = 0; CurrentIndex < numSides; ++CurrentIndex)
    {
        int NextIndex = (CurrentIndex + 1) % numSides;

        ColoredTriangle t1;
        //set vertex locations
        t1.VertexLocations[0] = circleStart[CurrentIndex];
        t1.VertexLocations[1] = circleStart[NextIndex];
        t1.VertexLocations[2] = circleEnd[CurrentIndex];
        //assign colors
        t1.VertexColors[0] = color1;
        t1.VertexColors[1] = color1;
        t1.VertexColors[2] = color2;
        //calculate normals
        glm::vec3 v1 = t1.VertexLocations[1] - t1.VertexLocations[0];
        glm::vec3 v2 = t1.VertexLocations[2] - t1.VertexLocations[0];

        glm::vec3 normal = glm::cross(v1, v2);
        for(auto& Normal : t1.VertexNormals) {
            Normal = normal;
        }

        ColoredTriangle t2;
        t2.VertexLocations[0] = circleEnd[NextIndex];
        t2.VertexLocations[1] = circleEnd[CurrentIndex];
        t2.VertexLocations[2] = circleStart[NextIndex];
        t2.VertexColors[0] = color2;
        t2.VertexColors[1] = color2;
        t2.VertexColors[2] = color1;
        v1 = t2.VertexLocations[1] - t2.VertexLocations[0];
        v2 = t2.VertexLocations[2] - t2.VertexLocations[0];
        normal = glm::cross(v1, v2);
        for(auto& Normal : t2.VertexNormals) {
            Normal = normal;
        }

        triangles->AddTriangle(t1);
        triangles->AddTriangle(t2);
    }
}

void Turtle::StartBranch()
{
    BranchStack.Push({bIsDefiningPolygon, CurrentWidth, CurrentTransform, CurrentColor});
}

void Turtle::CompleteBranch()
{
    StateData Data = BranchStack.Pop();
    bIsDefiningPolygon = Data.bIsDefiningPolygon;
    CurrentWidth = Data.CurrentWidth;
    CurrentTransform = Data.CurrentTransform;
    CurrentColor = Data.CurrentColor;
}

void Turtle::StartPolygon()
{
    //set ourselves to be
    bIsDefiningPolygon = true;
    PolygonStartTransform = CurrentTransform;
    polygonVertices.push_back(CurrentTransform.GetLocation());
}

void Turtle::CompletePolygon(ColoredTriangleList* triangles)
{
    bIsDefiningPolygon = false;

    //remove last vertex if it matches the first, as we're going to iterate through them
    if(polygonVertices.size() > 1 && polygonVertices[0] == polygonVertices[polygonVertices.size()-1])
    {
        polygonVertices.pop_back();
    }

    //use static polygon color
    glm::vec3 color = {0.1f, 0.3f, 0.2f};

    //placeholder top and bottom triangles we're working with
    //top triangle always has the same start vertex, as we're filling a polygon
    //color also stays the same throughout
    ColoredTriangle TopTriangle, BottomTriangle;
    TopTriangle.VertexLocations[0] = polygonVertices[0];
    TopTriangle.VertexColors[0] = BottomTriangle.VertexColors[0] =  color;
    TopTriangle.VertexColors[1] = BottomTriangle.VertexColors[1] =  color;
    TopTriangle.VertexColors[2] = BottomTriangle.VertexColors[2] =  color;

    //for each polygon other than the first, create a triangle with the next two vertices in the sequence
    for(int i = 1; i < polygonVertices.size()-1; i++)
    {
        //set vertices 1 and 2 of the triangle to polgon vertices i and i+1, and keep track of them
        glm::vec3 v1 = TopTriangle.VertexLocations[1] = polygonVertices[i];
        glm::vec3 v2 = TopTriangle.VertexLocations[2] = polygonVertices[i + 1];

        //subtract constant VertexLocations[0] from both and calculate normal based on it
        v1 -= TopTriangle.VertexLocations[0];
        v2 -= TopTriangle.VertexLocations[0];
        glm::vec3 TopTriangleNormal = glm::cross(v1, v2);

        //set normals for both triangles, set location of bottom triangle to slightly below top triangle, and flip winding
        for(int j = 0; j < 3; j++)
        {
            TopTriangle.VertexNormals[j] = TopTriangleNormal;
            BottomTriangle.VertexNormals[j] = -TopTriangleNormal;
            BottomTriangle.VertexLocations[j] = TopTriangle.VertexLocations[2-j] - (PolygonStartTransform.GetUpVector() * .001f);
        }

        //add top and bottom triangles to our list
        triangles->AddTriangle(TopTriangle);
        triangles->AddTriangle(BottomTriangle);
    }

    polygonVertices.clear();
}

void Turtle::TurnAround()
{
    CurrentTransform.AdjustYaw(180);
}

void Turtle::RotateToVertical()
{
    //rolls the turtle around its own axis so that vector L-> pointing to the left of the turtle is brought to a horizoental position
    //consequently, the branch plane is 'closest to the horizontal plane'.
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

    const glm::vec3 NewUpDir = glm::normalize(glm::cross(CurrentTransform.GetForwardVector(), NewLeftDir));
    const glm::mat3 RotationMatrix(CurrentTransform.GetForwardVector(), NewUpDir, NewLeftDir);
    CurrentTransform.SetRotation(RotationMatrix);
}

