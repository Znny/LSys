//
// Created by Ryanc on 10/28/2024.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

enum class ECameraProjectionMode
{
    Perspective = 0,
    Orthographic
};

class Camera : public Transform
{
public:
    Camera() = default;
    explicit Camera(double Width = 800,
                    double Height = 600,
                    double Near = 0.1,
                    double Far = 1000.0,
                    ECameraProjectionMode Mode = ECameraProjectionMode::Perspective,
                    double VerticalFOV = 45.0 );

    void SetProjectionMode(const ECameraProjectionMode& Mode);
    void SetClipDimensions(const double Width,const double Height, const double Near, const double Far);
    void SetClipWidth(const double Width);
    void SetClipHeight(const double Height);
    void SetClipNear(const double Near);
    void SetClipFar(const double Far);

    const glm::mat4& GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix();

protected:
    void UpdateProjectionMatrix();

private:
    ECameraProjectionMode ProjectionMode = ECameraProjectionMode::Perspective;
    double VerticalFieldOfView = 45.0;
    double ClipWidth = 800.0;
    double ClipHeight = 600.0;
    double NearClipDistance = 0.1;
    double FarClipDistance = 1000.0;
    glm::mat4 ProjectionMatrix = glm::mat4();

};


#endif //CAMERA_H
