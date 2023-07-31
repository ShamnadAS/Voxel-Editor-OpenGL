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
uniform int borderOn = 1;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
float filteredGrid( in vec2 p, in vec2 dpdx, in vec2 dpdy );

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    vec3 mate = vec3(0.0);
    mate = vec3(1.0) * filteredGrid(TexCoords, dFdx(TexCoords), dFdy(TexCoords));

    if(borderOn == 1)
    {
        // float t = 0.015;
        // if(TexCoords.x > t && TexCoords.x < 1.0 - t && TexCoords.y > t && TexCoords.y < 1.0 - t)
        // {
        //     FragColor = vec4(result, 1.0) * vec4(mate, 1.0);
        // }
        // else
        // {   
        //     FragColor = vec4(1.0, 1.0, 1.0, 1.0) * vec4(mate, 1.0);
        // }
        FragColor = vec4(result, 1.0) * vec4(mate, 1.0);
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

   return ambient + diffuse;
}

float filteredGrid( in vec2 p, in vec2 dpdx, in vec2 dpdy )
{
    const float N = 30.0;
    vec2 w = max(abs(dpdx), abs(dpdy));
    vec2 a = p + 0.5*w;                        
    vec2 b = p - 0.5*w;           
    vec2 i = (floor(a)+min(fract(a)*N,1.0)-
              floor(b)-min(fract(b)*N,1.0))/(N*w);
    return (1.0-i.x)*(1.0-i.y);
}