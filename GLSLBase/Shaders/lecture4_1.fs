#version 450

in vec4 v_Color;

layout(location=0) out vec4 FragColor;

const float PI = 3.141592;

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
	float temp = sin(dis * 12 * PI);
	return vec4(temp);
}

void main()
{
	// �������� 0.5��ŭ ������ �Ÿ� // Ư�� ���ǿ� ���� ������ ��Ʈ���Ѵ�.
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

	// [����� �뵵]
	// FragColor = vec4(dis);
}