
#version 330 core

struct Material{
    sampler2D diffuse;
};

uniform Material material;

out vec4 FragColor;

in vec2 TexCoords;

void main()
{
    FragColor = texture(material.diffuse, TexCoords);

}
