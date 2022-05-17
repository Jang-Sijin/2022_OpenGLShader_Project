#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Color;
in vec2 v_TexCoord;

const float PI = 3.141592;

vec4 CrossPattern()
{
	vec4 returnValue = vec4(1, 1, 1, 1); // ��� vec4(a,g,b,a)
	float dis = distance(vec2(0,0), v_TexCoord); // v_TexCoord: (x, y) ��ǥ

	return returnValue;
}

vec4 DrawCircle()
{
	float dis = distance(vec2(0,0), v_TexCoord);
	if(dis < 0.05)
	{
		return v_Color;
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}

void main()
{
	// [�� ����� ��ƼŬ ���� ���]
	// FragColor = DrawCircle();
	
	FragColor = CrossPattern();
}