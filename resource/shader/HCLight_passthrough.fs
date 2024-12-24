#version 400

in vec3 color;
in vec3 fragPosition;
in vec3 normal;

out vec4 frag_colour;

vec3 lightPos = vec3(100.0, 1.0, 1.0);   // Light position in world space
vec3 lightColor = vec3(1.0);; // Light color
vec3 ambientColor = vec3(0.9); // Optional ambient light (can hardcode if desired)

void main()
{
    // Ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combine lighting with object color
    vec3 lighting = (ambient + diffuse) * color;
    frag_colour = vec4(lighting, 1.0);
};
