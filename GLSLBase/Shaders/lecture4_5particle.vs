#version 450

// in는 atrribute라는 뜻이다.
in vec3 a_Position;
in vec4 a_Color;

out vec4 v_Color;
out vec4 v_ColorOverride;

void main()
{
	gl_Position = vec4(a_Position, 1); // -0.5 ~ 0.5, x, y 각 축별로 좌표가 넘어온다.

	vec3 newValue = a_Position + vec3(0.5, 0.5, 0.0);
	v_Color = vec4(newValue, 1);
	v_ColorOverride = a_Color;
}
