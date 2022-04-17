#version 450

// in는 atrribute라는 뜻이다.
in vec3 a_Position;
in vec4 a_Color;

uniform float u_Time;

out vec4 v_Color;

void main()
{
	gl_Position = vec4(a_Position * u_Time, 1); // 외부 u_Time 값(시간)에 따라 포지션의 위치가 줄어들게 만듦.
	v_Color = a_Color;
}