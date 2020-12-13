#version 330 core

in vec3 inPosition;
in vec2 inTexCoord;

out vec2 exTexCoord;

uniform mat4 ModelMatrix;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

void main(void)
{
	exTexCoord = inTexCoord;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * ModelMatrix * MCPosition;
}