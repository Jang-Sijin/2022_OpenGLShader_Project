#version 450

// in�� atrribute��� ���̴�.
in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_AmpTime;
in float a_PeriodTime;

uniform float u_Time;
uniform vec3 u_Accel;

bool bLoop = true; // ����.. (true, false�� ���� �ݺ������� ������ų ���ΰ�?)

float g_PI = 3.14;

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

		float amp = a_AmpTime;			// ����
		float period = a_PeriodTime;	// ��
		newPos.x = a_Position.x + (a_Velocity.x * time) + (0.5f * u_Accel.x * timeSquare);
		newPos.y = a_Position.y + amp * sin(period * time * 2.0 * g_PI);
		newPos.z = 0;
	}
	else
	{
		newPos = vec3(-1000000, -1000000, -1000000);
	}
	
	gl_Position = vec4(newPos, 1);
}