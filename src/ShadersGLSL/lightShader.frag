#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
    vec3 color;
};

struct Light {
    //vec3 position; // we don't need position when working with direction lighting
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
//TEX
//in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{

    //Ambient lighting TEX
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 ambient = light.ambient * material.color;
    //Diffuse lighting
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-light.direction); //direction lighting
    //vec3 lightDir = normalize(light.position - FragPos); 
    
    float diff = max(dot(norm, lightDir), 0.0);
    //TEX
    //vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * material.color;
    //Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    //TEX
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * spec * vec3(1.0f);

    //vec3 emission = texture(material.emission, TexCoords).rgb;

    FragColor = vec4(ambient + diffuse + specular , 1.0);
}
