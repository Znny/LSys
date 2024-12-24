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
            VertexNormals[i] = glm::vec3(0);
        }
    }

    glm::vec3 VertexLocations[3]{};
    glm::vec3 VertexColors[3]{};
    glm::vec3 VertexNormals[3]{};

    void RandomizeColors()
    {
        for ( glm::vec3& VertexColor : VertexColors)
        {
            VertexColor.r = (rand() % 1000) / 1000.0f;
            VertexColor.g = (rand() % 1000) / 1000.0f;
            VertexColor.b = (rand() % 1000) / 1000.0f;
        }
    }
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

        for(int i = 0; i < 3; i++)
        {
            glm::vec3& vert = Triangle.VertexLocations[i];
            //track min and max values for vertices in all three dimensions (finding bounding box)
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

                /*
                BoundingBoxMin.x = vert.x < BoundingBoxMin.x ? vert.x : BoundingBoxMin.x;
                BoundingBoxMax.x = vert.x > BoundingBoxMax.x ? vert.x : BoundingBoxMax.x;
                BoundingBoxMin.y = vert.y < BoundingBoxMin.y ? vert.y : BoundingBoxMin.y;
                BoundingBoxMax.y = vert.y > BoundingBoxMax.y ? vert.y : BoundingBoxMax.y;
                BoundingBoxMin.z = vert.z < BoundingBoxMin.z ? vert.z : BoundingBoxMin.z;
                BoundingBoxMax.z = vert.z > BoundingBoxMax.z ? vert.z : BoundingBoxMax.z;
                */
            }
        }
    }

    long long int NumTriangles = 0;
    long long int MaxTriangles = 0;

    static constexpr float FLOAT_MIN = 1.175494351E-38;
    static constexpr float FLOAT_MAX = 3.402823466E+38;
    glm::vec3 BoundingBoxMin = glm::vec3(FLOAT_MAX);
    glm::vec3 BoundingBoxMax = glm::vec3(FLOAT_MIN);
    ColoredTriangle* TriData = nullptr;
};
#endif //COLOREDTRIANGLE_H
