#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D radarTexture;
uniform float interpolationFactor;

void main()
{ 
    vec4 screenColor = texture(screenTexture, TexCoords);
    vec4 radarColor = texture(radarTexture, TexCoords);
    vec4 texColor = mix(radarColor, screenColor, interpolationFactor);
    if(texColor.a == 0)
        discard;
    FragColor = texColor;
} 