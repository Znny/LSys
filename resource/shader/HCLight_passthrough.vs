#version 400

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 ViewProjectionMatrix;

out vec3 color;
out vec3 fragPosition;
out vec3 normal;

void main()
{
    color = vertexColor;

    // Transform vertex position to world space
    fragPosition = vertexPosition;

    normal = vertexNormal;

    gl_Position = ViewProjectionMatrix * vec4(fragPosition, 1.0);
};
