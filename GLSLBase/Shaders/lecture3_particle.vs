#version 450

// in�� atrribute��� ���̴�.
in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;
uniform vec3 u_Accel;

bool bLoop = true; // ����.. (true, false�� ���� �ݺ������� ������ų ���ΰ�?)

void main()
{
	float time = u_Time - a_EmitTime;
	float timeSquare = time * time;

	vec3 newPos;
	if(time > 0)
	{
		float temp = time / a_LifeTime;
		float fractional = fract(temp); // ���̴� ��ü ���� - �Ҽ��� �Ʒ� �κи� ����.
		time = fractional * a_LifeTime;
		timeSquare = time * time;
		newPos = a_Position + (a_Velocity * time) + (0.5f * u_Accel * timeSquare);
	}
	else
	{
		newPos = vec3(-1000000, -1000000, -1000000);
	}
	
	gl_Position = vec4(newPos, 1);
}