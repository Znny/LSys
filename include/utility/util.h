//
// Created by Ryan on 12/23/2024.
//

#pragma once

#include <vector>

#include "glm.hpp"

//take an HSV-based glm::vec3 and return an RGB based glm::vec3
glm::vec3 HSVtoRGB(const glm::vec3& hsv);

//take an RGB-based glm::vec3 and return an HSV based glm::vec3
glm::vec3 RGBtoHSV(const glm::vec3& rgb);

//
std::vector<glm::vec3> GenerateSphere(glm::vec3 Center, float Radius, unsigned int LongitudeSegments, unsigned int LatitudeSegments);
