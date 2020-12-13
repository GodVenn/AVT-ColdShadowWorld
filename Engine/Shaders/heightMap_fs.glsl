#version 330 core

in vec4 exColor;
in vec2 exTexCoord;

out vec4 FragmentColor;

uniform sampler2D Texture;

void main(void)
{
	FragmentColor = texture(Texture, exTexCoord);
	//FragmentColor = vec4(exTexCoord,0,1);
}