//
// Created by Ryanc on 10/28/2024.
//

#include "Camera.h"

void Camera::SetProjectionMode(const CameraProjectionMode &Mode)
{
    ProjectionMode = Mode;
    UpdateProjectionMatrix();
}

void Camera::SetClipDimensions(const double Width, const double Height, const double Near, const double Far)
{
    ClipWidth = Width;
    ClipHeight = Height;
    NearClipDistance = Near;
    FarClipDistance = Far;
    UpdateProjectionMatrix();
}

void Camera::SetClipWidth(const double Width)
{
    ClipWidth = Width;
    UpdateProjectionMatrix();
}

void Camera::SetClipHeight(const double Height)
{
    ClipHeight = Height;
    UpdateProjectionMatrix();
}

void Camera::SetClipNear(const double Near)
{
    NearClipDistance = Near;
    UpdateProjectionMatrix();
}

void Camera::SetClipFar(const double Far)
{
    FarClipDistance = Far;
    UpdateProjectionMatrix();
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return ProjectionMatrix;
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
    return ProjectionMatrix * GetMatrix();
}

void Camera::UpdateProjectionMatrix()
{
    ProjectionMatrix =
        ProjectionMode == CameraProjectionMode::Perspective
            ? glm::perspective(VerticalFieldOfView, ClipWidth / ClipHeight, NearClipDistance, FarClipDistance)
            : glm::ortho(0.0, ClipWidth, 0.0, ClipHeight, NearClipDistance, FarClipDistance);
}
