#version 400

in vec3 color;
in vec3 fragPosition;
in vec3 normal;

out vec4 frag_colour;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform float ambientStrength;

void main()
{
    // Ambient lighting
    vec3 ambient = ambientStrength * ambientColor;

    // Diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine lighting with object color
    vec3 lighting = max(ambient, diffuse) * color;
    frag_colour = vec4(lighting, 1.0);
};
