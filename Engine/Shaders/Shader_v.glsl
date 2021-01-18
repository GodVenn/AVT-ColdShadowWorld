#version 330 core

in vec3 in_Position;
in vec4 in_Color;
out vec4 ex_Color;

uniform float contrast;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;

void main(void)
{
	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * ModelMatrix * vec4(in_Position,1);
	ex_Color = in_Color * contrast;
}