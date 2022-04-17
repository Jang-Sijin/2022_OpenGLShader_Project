#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;

uniform vec4 u_Color = vec4(1, 1, 1, 1);

void main()
{
	FragColor = vec4(u_Color * v_Color);
}