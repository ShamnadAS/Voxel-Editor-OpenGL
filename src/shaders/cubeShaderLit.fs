#version 460 core
out vec4 FragColor;

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform DirLight dirLight;
uniform vec3 viewPos;
uniform vec3 color;
uniform int borderOn;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    if(borderOn == 1)
    {
        float t = 0.01;
        if(TexCoords.x > t && TexCoords.x < 1.0 - t && TexCoords.y > t && TexCoords.y < 1.0 - t)
        {
            FragColor = vec4(result, 1.0);
        }
        else
        {   
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    }
    else
    {
        FragColor = vec4(result, 1.0);
    }
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
   vec3 lightDir = normalize(vec3(light.direction));

   //ambient
   vec3 ambient = light.ambient * color;

   //diffuse
   float diff = max(dot(normal, -lightDir), 0.0);
   vec3 diffuse = light.diffuse * diff * color;  

   //specular	
//    vec3 reflectDir = reflect(lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = spec * light.specular * texture(material.specular, TexCoords).rgb;

   return ambient + diffuse;
}