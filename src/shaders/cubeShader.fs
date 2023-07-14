#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 color;
uniform int borderOn = 1;

void main()
{
    if(borderOn == 1)
    {
        float t = 0.01;
        if(TexCoords.x > t && TexCoords.x < 1.0 - t && TexCoords.y > t && TexCoords.y < 1.0 - t)
        {
            FragColor = vec4(color, 1.0);
        }
        else
        {   
            FragColor = vec4(1.0, 1.0, 1.0, 1.0);
        }
    }
    else
    {
        FragColor = vec4(color, 1.0);
    }
}