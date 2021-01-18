#version 330 core
in vec2 inPosition;
in vec2 inTexcoord;
uniform vec2 translationVector;
uniform vec2 scaleMultiplier;

out vec2 TexCoords;

void main()
{
    float positionX = inPosition.x * scaleMultiplier.x + translationVector.x;
    float positionY = inPosition.y * scaleMultiplier.y + translationVector.y;

    gl_Position = vec4(positionX, positionY, 0.0, 1.0); 
    TexCoords = inTexcoord;
} 