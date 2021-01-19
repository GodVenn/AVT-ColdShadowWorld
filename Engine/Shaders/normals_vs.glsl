#version 330 core

in vec3 inPosition;
in vec2 inTexcoord;
in vec3 inNormal;

out vec3 exNormal;

uniform mat4 ModelMatrix;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;


void main(void)
{
	exNormal = inNormal;

	vec4 MCPosition = vec4(inPosition, 1.0);
	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * ModelMatrix * MCPosition;
}