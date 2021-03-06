#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec3 inNormal;

out vec4 exShadowVertex;
out vec3 exPosition;
out vec3 exNormal;

out vec3 exEyeVector;			// Point of view
out vec3 exReflectionVector;	// To compute the specular tern
out float exDiffuseIntensity;	// Difusse intensity 

uniform mat4 ModelMatrix;
layout(std140) uniform ViewProjectionMatrices
{
	uniform mat4 ViewMatrix;
	uniform mat4 ProjectionMatrix;
} Camera;
uniform vec3 LightPosition;

uniform mat4 ViewLightMatrix;
uniform mat4 ProjLightMatrix;


void main(void)
{	
	vec4 vertex =  vec4(inPosition, 1.0);
	// Convert vertex, normal and light pos to eye space
	mat4 ModelViewMatrix = Camera.ViewMatrix * ModelMatrix;
	mat3 NormalMatrix = mat3(transpose(inverse(ModelViewMatrix)));

	vec3 V = vec3(ModelViewMatrix * vertex);
	vec3 N = normalize(NormalMatrix * inNormal);
	vec3 Lpos = vec3(Camera.ViewMatrix * vec4(LightPosition,1));

	vec3 L = normalize(Lpos - V); // Light vector
	
	exEyeVector = -V;
	exReflectionVector = reflect(-L,N);
	// In order to interpolate between the cool and warn diffuse component
	exDiffuseIntensity = dot(L,N) * 0.5  + 0.5; // min 0.5, max 1

	exPosition =  vec3(ModelMatrix * vertex);
	exNormal = inNormal;
	
	// Vertex Pos from the ligth point of view
	exShadowVertex = ProjLightMatrix * ViewLightMatrix * ModelMatrix * vertex;
	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * ModelMatrix * vertex;
}