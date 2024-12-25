//
// Created by Ryan on 12/23/2024.
//

#include "utility/util.h"

#include <cmath> // For fmod function
#include <algorithm>
#include <vector>

glm::vec3 HSVtoRGB(const glm::vec3& hsv) {
    glm::vec3 rgb;
    auto h = static_cast<float>(fmod(hsv.r, 360.0f)); // Ensure hue is within [0, 360)
    if (h < 0.0f) h += 360.0f;    // Correct negative hue values
    h /= 60.0f;                   // Sector (0 to 5)

    int i = static_cast<int>(h) % 6; // Clamp sector to [0, 5]
    float f = h - static_cast<float>(i);                // Fractional part of h
    float p = hsv.b * (1 - hsv.g);
    float q = hsv.b * (1 - hsv.g * f);
    float t = hsv.b * (1 - hsv.g * (1 - f));

    switch (i) {
        case 0:
            rgb.r = hsv.b;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = hsv.b;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = hsv.b;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = hsv.b;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = hsv.b;
            break;
        case 5:
        default:
            rgb.r = hsv.b;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}


glm::vec3 RGBtoHSV(const glm::vec3& rgb) {
    glm::vec3 hsv;
    float max = std::max({rgb.r, rgb.g, rgb.b});
    float min = std::min({rgb.r, rgb.g, rgb.b});
    float delta = max - min;

    // Compute hue
    if (delta == 0.0f) {
        hsv.r = 0.0f; // Hue is undefined when there's no color variation
    } else if (max == rgb.r) {
        hsv.r = static_cast<float>(60.0 * fmod((rgb.g - rgb.b) / delta, 6.0));
    } else if (max == rgb.g) {
        hsv.r = 60.0f * ((rgb.b - rgb.r) / delta + 2.0f);
    } else { // max == rgb.b
        hsv.r = 60.0f * ((rgb.r - rgb.g) / delta + 4.0f);
    }

    if (hsv.r < 0.0f) {
        hsv.r += 360.0f; // Ensure hue is in [0, 360)
    }

    // Compute saturation
    hsv.g = (max == 0.0f) ? 0.0f : delta / max;

    // Compute value (brightness)
    hsv.b = max;

    return hsv;
}

// Function to generate a sphere
std::vector<glm::vec3> GenerateSphere(glm::vec3 Center, float Radius, unsigned int LongitudeSegments, unsigned int LatitudeSegments) {
    std::vector<glm::vec3> vertices;

    for (unsigned int lat = 0; lat <= LatitudeSegments; ++lat) {
        float theta = glm::radians(180.0f * static_cast<float>(lat) / LatitudeSegments); // Latitude angle
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (unsigned int lon = 0; lon <= LongitudeSegments; ++lon) {
            float phi = glm::radians(360.0f * static_cast<float>(lon) / LongitudeSegments); // Longitude angle
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            // Calculate vertex position
            glm::vec3 vertex = Center + Radius * glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            vertices.push_back(vertex);
        }
    }

    std::vector<glm::vec3> triangles;

    // Create triangles from the sphere vertices
    for (unsigned int lat = 0; lat < LatitudeSegments; ++lat) {
        for (unsigned int lon = 0; lon < LongitudeSegments; ++lon) {
            unsigned int first = lat * (LongitudeSegments + 1) + lon;
            unsigned int second = first + LongitudeSegments + 1;

            // First triangle of the quad
            triangles.push_back(vertices[first]);
            triangles.push_back(vertices[second]);
            triangles.push_back(vertices[first + 1]);

            // Second triangle of the quad
            triangles.push_back(vertices[second]);
            triangles.push_back(vertices[second + 1]);
            triangles.push_back(vertices[first + 1]);
        }
    }

    return triangles;
}
