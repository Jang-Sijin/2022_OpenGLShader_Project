#version 450

// in�� atrribute��� ���̴�.
in vec3 a_Position;
in vec4 a_Color;

uniform float u_Time;

out vec4 v_Color;

void main()
{
	gl_Position = vec4(a_Position * u_Time, 1); // �ܺ� u_Time ��(�ð�)�� ���� �������� ��ġ�� �پ��� ����.
	v_Color = a_Color;
}