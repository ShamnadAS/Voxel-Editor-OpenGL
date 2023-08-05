#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec3 color;
uniform int borderOn = 1;

float filteredGrid( in vec2 p, in vec2 dpdx, in vec2 dpdy );

void main()
{
    vec3 mate = vec3(0.0);
    mate = vec3(1.0) * filteredGrid(TexCoords, dFdx(TexCoords), dFdy(TexCoords));

    if(borderOn == 1)
    {
        FragColor = vec4(color, 1.0) * vec4(mate, 1.0);
    }
    else
    {
        FragColor = vec4(color, 1.0);
    }
}

float filteredGrid( in vec2 p, in vec2 dpdx, in vec2 dpdy )
{
    float N = 50.0;
    vec2 w = max(abs(dpdx), abs(dpdy));
    vec2 a = p + 0.5*w;                        
    vec2 b = p - 0.5*w;  

    float delta = 0.002;

    const float k = ((N + 1) / N ) - 2 * delta;

    vec2 i = (floor((a * k) + delta)+min(fract((a * k) + delta)*N,1.0)-
              floor((b * k) + delta)-min(fract((b * k) + delta)*N,1.0))/(N*w);

    return (1.0 - i.x)*(1.0 - i.y); 
}