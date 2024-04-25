#version 330 core


uniform vec3 lightPos;
uniform vec3 color;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;

void main()
{

    vec3 ambient = vec3(0.2f, 0.2f, 0.2f) * color; //ambient vec
    
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos); 
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(0.7f, 0.7f, 0.7f) * diff * color; //diffuse vec
    //Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    FragColor = vec4(ambient + diffuse , 1.0); //satelite hasn't specular lighting
}
