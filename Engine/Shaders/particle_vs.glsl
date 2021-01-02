#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

out VS_OUT {
    vec4 color;
} vs_out;


void main()
{
    if(inColor.w == 0.0){ //Discard invisible particles
        return;
    }
	mat4 modelViewMatrix = Camera.ViewMatrix;
    vs_out.color = inColor;
    gl_Position = modelViewMatrix * vec4(inPosition.xyz, 1.0);
}