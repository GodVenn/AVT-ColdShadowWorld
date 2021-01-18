#version 330 core
out vec4 FragColor;

in vec4 fColor;
in vec2 texcoord;

uniform sampler2D snowText;

void main()
{
    if(fColor.w == 0.0)
        discard;

    vec2 uv = texcoord;
	vec4 t = texture(snowText, uv);
	FragColor = t * fColor;
}