#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
};

uniform Material material;

out vec4 FragColor;

in vec2 TexCoords;

void main()
{
    FragColor = vec4(vec3(texture(material.diffuse, TexCoords)), 1.0); //set all 4 vector values to 1.0
}
