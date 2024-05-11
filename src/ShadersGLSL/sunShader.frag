#version 330 core

struct Material{
    sampler2D diffuse;
};

uniform Material material;

out vec3 FragColor;

in vec2 TexCoords;

void main()
{
    FragColor =vec3(texture(material.diffuse, TexCoords)); //set all 4 vector values to 1.0
}
