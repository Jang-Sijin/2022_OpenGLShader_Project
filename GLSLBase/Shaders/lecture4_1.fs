#version 450

in vec4 v_Color;

layout(location=0) out vec4 FragColor;

const float PI = 3.141592;

vec4 CrossPattern()
{
	// vec4 returnValue = vec4(sin(v_Color.x * (2 * PI))); // 주기가 2PI인 sin 함수를 활용하여 철조망 디자인(패턴)을 만든다. // x축에만 적용
	
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
	float temp = sin(dis * 12 * PI);
	return vec4(temp);
}

void main()
{
	// 원점에서 0.5만큼 떨어진 거리 // 특정 조건에 따라 색상을 컨트롤한다.
	float dis = distance(v_Color.xy, vec2(0.5, 0.5));

	if(dis < 0.5)
	{
		FragColor = v_Color;
	}
	else
	{
		FragColor = vec4(0, 0, 0, 0);
	}
	
	if(v_Color.y > 0.5)
	{
		FragColor = v_Color;
	}
	else
	{
		FragColor = vec4(0, 0, 0, 0);
	}

	if(v_Color.y < 0.5)
	{
		FragColor = v_Color;
	}
	else
	{
		FragColor = vec4(0, 0, 0, 0);
	}

	if(dis > 0.48 && dis < 0.5)
	{
		FragColor = v_Color;
	}
	else
	{
		FragColor = vec4(0, 0, 0, 0);
	}

	FragColor = CrossPattern();

	FragColor = DrawMultipleCircles();

	// [디버깅 용도]
	// FragColor = vec4(dis);
}