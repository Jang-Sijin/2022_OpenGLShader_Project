#version 450

// in�� atrribute��� ���̴�.
in vec3 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 1);
}
