//
// Created by Ryanc on 10/10/2024.
//

#include "Transform.h"

const glm::vec3 Transform::WorldForward{0.0f, 0.0f, -1.0f};
const glm::vec3 Transform::WorldRight{1.0f, 0.0f, 0.0f};
const glm::vec3 Transform::WorldUp{0.0f, 1.0f, 0.0f};

void Transform::Rotate(const glm::vec3 &AxisOfRotation, float Degrees)
{
    glm::quat rotationQuat = glm::angleAxis(glm::radians(Degrees), glm::normalize(AxisOfRotation));
    Rotation = glm::normalize(rotationQuat * Rotation);
}

void Transform::AdjustYaw(float Angle)
{
    //do the rotation
    Rotate(GetUpVector(), Angle);
}

void Transform::AdjustPitch(float Angle)
{
    //do the rotation
    Rotate(GetRightVector(), Angle);
}

void Transform::AdjustRoll(float Angle)
{
    //do the rotation
    Rotate(GetForwardVector(), Angle);
}

