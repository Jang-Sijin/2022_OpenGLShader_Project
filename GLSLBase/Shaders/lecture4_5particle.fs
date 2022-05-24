#version 450

in vec4 v_Color;
in vec4 v_ColorOverride;

layout(location=0) out vec4 FragColor;

const float PI = 3.141592;

uniform vec3 u_Points[10];
uniform float u_Time;
uniform float u_LifeTime;

vec4 CrossPattern()
{
	// vec4 returnValue = vec4(sin(v_Color.x * (2 * PI))); // �ֱⰡ 2PI�� sin �Լ��� Ȱ���Ͽ� ö���� ������(����)�� �����. // x�࿡�� ����
	
	float XAxis = sin(10 * (v_Color.x * 2 * PI + PI/4.0f));
	float YAxis = sin(10 * (v_Color.y * 2 * PI + PI/4.0f));
	vec4 returnValue;
	returnValue.x = XAxis;
	returnValue.y = YAxis;

	return returnValue;
}

vec4 DrawMultipleCircles()
{
	float dis = distance(v_Color.xy, vec2(0.5, 0.5)); // 0 ~ 0.5
	float temp = sin(6 * dis * 2 * PI);
	return vec4(temp);
}

vec4 DrawCircles()
{
	vec4 returnColor = vec4(0);

	for(int i = 0; i < 10; i++)
	{
		float dis = distance(u_Points[i].xy, v_Color.xy);
		float circlePattern = sin(10 * dis * 4 * PI - (u_Time * 100));

		if(dis < u_Time)
			returnColor += vec4(circlePattern);
		// if(dis < 0.1f) // �������� 0.1�� ���� �����.
		// {
		// 		returnColor = vec4(1);
		// }
	}

	return returnColor;
}

vec4 RaderCircle()
{
	float d = distance(vec2(0.5, 0), v_Color.xy);
	float sinValue = sin(2 * d * 2 * PI - u_Time * 75); // uniform���� ���� ��Ʈ���� �� �ְ� ���� �� �ִ�.
	sinValue = clamp(pow(sinValue, 3), 0, 1);
	vec4 returnColor = vec4(0.5 * sinValue);

	for(int i = 0; i < 10; i++)
	{
		float dTemp = distance(u_Points[i].xy, v_Color.xy);
		if(dTemp < 0.1)
			returnColor += vec4(0, 20 * sinValue * (0.1 - dTemp), 0, 0);
	}
	
	return returnColor;
}


void main()
{
	// �������� 0.5��ŭ ������ �Ÿ� // Ư�� ���ǿ� ���� ������ ��Ʈ���Ѵ�.
	//float dis = distance(v_Color.xy, vec2(0.5, 0.5));
	//
	//if(dis < 0.5)
	//{
	//	FragColor = v_Color;
	//}
	//else
	//{
	//	FragColor = vec4(0, 0, 0, 0);
	//}
	//
	//if(v_Color.y > 0.5)
	//{
	//	FragColor = v_Color;
	//}
	//else
	//{
	//	FragColor = vec4(0, 0, 0, 0);
	//}
	//
	//if(v_Color.y < 0.5)
	//{
	//	FragColor = v_Color;
	//}
	//else
	//{
	//	FragColor = vec4(0, 0, 0, 0);
	//}
	//
	//if(dis > 0.48 && dis < 0.5)
	//{
	//	FragColor = v_Color;
	//}
	//else
	//{
	//	FragColor = vec4(0, 0, 0, 0);
	//}

	//FragColor = CrossPattern();
	//FragColor = DrawMultipleCircles();
	//FragColor = DrawCircles();
	FragColor = RaderCircle() * v_ColorOverride;

	// [����� �뵵]
	// FragColor = vec4(dis);
}