#version 450

// in�� atrribute��� ���̴�.
in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_AmpTime;
in float a_PeriodTime;
in float a_RandomValue;
in vec4 a_Color;
// ����ȭ -> float �ϳ��� �Ѿ�ٴ� �̾߱�� GPU�� CPU�� ����� �Ѵٴ� ����̴�. (Vec4 ������ ��� Data�� �ѱ��.)
// uniform���� �ѹ��� �Ѱ��ִ� ���� ȿ������. (CPU���� GPU�� �ѱ�� Ƚ��, ����� ��������.)

out vec4 v_Color;

uniform float u_Time;
uniform vec3 u_Accel;

bool bLoop = true; // ����.. (true, false�� ���� �ݺ������� ������ų ���ΰ�?)

const float g_PI = 3.14;
const mat3 g_RotMat = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0); // ��GLSL�� �� �����̴�. �� (cos90, -sin90, 0, sin90, cos90, 0, 0, 0, 0)�� ������ ����� ���� �־��ָ� �ȴ�.
const vec3 g_Gravity = vec3(0, -0.5f, 0);

void main()
{
	float time = u_Time - a_EmitTime;
	float timeSquare = time * time;

	vec3 newPos;
	if(time > 0)
	{
		vec3 newAccel = g_Gravity + a_Velocity;

		// Parametric Function Shader
		newPos.x = sin(a_RandomValue * 2 * g_PI);
		newPos.y = cos(a_RandomValue * 2 * g_PI);
		newPos.z = 0;
		newPos = a_Position + newPos;

		float temp = time / a_LifeTime;
		float fractional = fract(temp); // ���̴� ��ü ���� - �Ҽ��� �Ʒ� �κи� ����.
		time = fractional * a_LifeTime;
		timeSquare = time * time;

		float amp = a_AmpTime;			// ����
		float period = a_PeriodTime;	// ��
		newPos = newPos + (newAccel * time) + (0.5f * u_Accel * timeSquare);
		
		vec3 rotVec = normalize(newAccel * g_RotMat);
		newPos = newPos + 0.1f * amp * rotVec * sin(period * time * 2.0 * g_PI);
		newPos.z = 0;

		v_Color = a_Color * (1.0 - fractional);
	}
	else
	{
		newPos = vec3(-1000000, -1000000, -1000000);
		v_Color = vec4(0, 0, 0, 0);
	}
	
	gl_Position = vec4(newPos, 1);
}