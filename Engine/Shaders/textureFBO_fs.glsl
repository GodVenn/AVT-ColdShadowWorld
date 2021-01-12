#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform vec4 u_Color;
void main()
{ 
    vec4 texColor = texture(screenTexture, TexCoords);
    if(texColor.a == 0)
        discard;
    FragColor = texColor * u_Color;
} 