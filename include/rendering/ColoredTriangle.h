//
// Created by Ryan Cosgrove on 10/20/2024.
//

#pragma once

#include <random>
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
            VertexNormals[i] = glm::vec3(0);
        }
    }

    glm::vec3 VertexLocations[3]{};
    glm::vec3 VertexColors[3]{};
    glm::vec3 VertexNormals[3]{};
};

/* ColoredTriangleList
 * an array of colored triangles
 */
struct ColoredTriangleList
{
    explicit ColoredTriangleList(int MaxTris)
    {
        NumTriangles = 0;
        MaxTriangles = MaxTris;
        TriData = static_cast<ColoredTriangle*>(malloc(MaxTriangles * sizeof(ColoredTriangle)));
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

        for(const glm::vec3& vert : Triangle.VertexLocations)
        {
            if(vert.x > BoundingBoxMax.x) {
                BoundingBoxMax.x = vert.x;
            } else if (vert.x < BoundingBoxMin.x) {
                BoundingBoxMin.x = vert.x;
            }
            if(vert.y > BoundingBoxMax.y) {
                BoundingBoxMax.y = vert.y;
            } else if (vert.y < BoundingBoxMin.y) {
                BoundingBoxMin.y = vert.y;
            }
            if(vert.z > BoundingBoxMax.z) {
                BoundingBoxMax.z = vert.z;
            } else if (vert.z < BoundingBoxMin.z) {
                BoundingBoxMin.z = vert.z;
            }
        }
    }
    void Clear()
    {
        NumTriangles = 0;
    }

    long long int NumTriangles = 0;
    long long int MaxTriangles = 0;

    static constexpr float FLOAT_MIN = 1.175494351E-38;
    static constexpr float FLOAT_MAX = 3.402823466E+38;
    glm::vec3 BoundingBoxMin = glm::vec3(FLOAT_MAX);
    glm::vec3 BoundingBoxMax = glm::vec3(FLOAT_MIN);
    ColoredTriangle* TriData = nullptr;
};
