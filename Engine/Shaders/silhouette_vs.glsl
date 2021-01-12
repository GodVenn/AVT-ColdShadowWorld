#version 330 core

in vec3 inPosition;
in vec3 inNormal;

uniform mat4 ModelMatrix;

layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

uniform float Offset; // to scale the object

void main(void)
{
	vec3 v = inPosition + normalize(inNormal) * Offset; // scale the vertex in the direction of the normal - more generic approach
	gl_Position =  Camera.ProjectionMatrix * Camera.ViewMatrix * ModelMatrix * vec4(v, 1.0);
}