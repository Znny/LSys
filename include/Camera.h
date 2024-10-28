//
// Created by Ryanc on 10/28/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

enum class CameraProjectionMode
{
    Perspective = 0,
    Orthographic
};

class Camera : public Transform
{
public:
    Camera() = default;

    void SetProjectionMode(const CameraProjectionMode& Mode);
    void SetClipDimensions(const double Width,const double Height, const double Near, const double Far);
    void SetClipWidth(const double Width);
    void SetClipHeight(const double Height);
    void SetClipNear(const double Near);
    void SetClipFar(const double Far);

    const glm::mat4& GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

protected:
    void UpdateProjectionMatrix();

private:
    CameraProjectionMode ProjectionMode = CameraProjectionMode::Perspective;
    double VerticalFieldOfView = 45.0;
    double ClipWidth = 800.0;
    double ClipHeight = 600.0;
    double NearClipDistance = 0.1;
    double FarClipDistance = 1000.0;
    glm::mat4 ProjectionMatrix = glm::mat4();

};


#endif //CAMERA_H
