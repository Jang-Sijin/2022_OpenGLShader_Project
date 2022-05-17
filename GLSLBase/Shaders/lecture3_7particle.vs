#version 450

// in는 atrribute라는 뜻이다.
in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_LifeTime;
in float a_AmpTime;
in float a_PeriodTime;
in float a_RandomValue;
in vec4 a_Color;
// 최적화 -> float 하나가 넘어간다는 이야기는 GPU와 CPU가 통신을 한다는 얘기이다. (Vec4 단위로 묶어서 Data를 넘긴다.)
// uniform으로 한번에 넘겨주는 것이 효율적임. (CPU에서 GPU로 넘기는 횟수, 비용이 적어진다.)

out vec4 v_Color;

uniform float u_Time;
uniform vec3 u_Accel;

bool bLoop = true; // 숙제.. (true, false에 따라서 반복적으로 생성시킬 것인가?)

const float g_PI = 3.14;
const mat3 g_RotMat = mat3(0, -1, 0, 1, 0, 0, 0, 0, 0); // ★GLSL은 열 기준이다. 즉 (cos90, -sin90, 0, sin90, cos90, 0, 0, 0, 0)의 순서로 행렬의 값을 넣어주면 된다.
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
		float fractional = fract(temp); // 쉐이더 자체 제공 - 소수점 아랫 부분만 뺀다.
		time = fractional * a_LifeTime;
		timeSquare = time * time;

		float amp = a_AmpTime;			// 높이
		float period = a_PeriodTime;	// 폭
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