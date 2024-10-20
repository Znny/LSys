//
// Created by Ryan Cosgrove on 10/20/2024.
//

#ifndef COLOREDTRIANGLE_H
#define COLOREDTRIANGLE_H

#include <glm/glm.hpp>

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
#endif //COLOREDTRIANGLE_H
