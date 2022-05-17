#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	//default settings
	glClearDepth(1.f);

	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders // 쉐이더 프로그램 ID
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_Lecture3Shader = CompileShaders("./Shaders/SolidRectCopy.vs", "./Shaders/SolidRectCopy.fs"); // 복사한 쉐이더 컴파일 오류 체크
	m_Lecture3ParticleShader = CompileShaders("./Shaders/lecture3_particle.vs", "./Shaders/lecture3_particle.fs");
	m_Lecture3ParticleShader1 = CompileShaders("./Shaders/lecture3_6particle.vs", "./Shaders/lecture3_6particle.fs");
	m_Lecture3ParticleShader2 = CompileShaders("./Shaders/lecture3_7particle.vs", "./Shaders/lecture3_7particle.fs");
	m_ParticleShaderLectrue4_1 = CompileShaders("./Shaders/lecture4_1.vs", "./Shaders/lecture4_1.fs");
	m_ShaderParticle_Lecture4_2 = CompileShaders("./Shaders/lecture4_2particle.vs", "./Shaders/lecture4_2particle.fs");
	m_ShaderParticle_Lecture4_3 = CompileShaders("./Shaders/lecture4_3particle.vs", "./Shaders/lecture4_3particle.fs");

	//Create VBOs
	CreateVertexBufferObjects();

	//Create Particles
	CreateParticle(1000); // 파티클(쿼드)를 1000개 생성한다. lecture3_3
	CreateParticleAnimation5(1000);
	CreateParticleAnimation5_Transform(1000);
	
	//Create Particles - Vertex Shader & Fragment Shader
	CreateVertexBufferObjectsLecture4_1();

	// Lecture4_2 - Create Particles - Vertex Shader & Fragment Shader
	CreateParticleAnimation_Lecture4_2(1000);
	// Lecture4_3 - Create Particles - Vertex Shader & Fragment Shader
	CreateParticleAnimation_Lecture4_3(1000);

	//Initialize camera settings
	m_v3Camera_Position = glm::vec3(0.f, 0.f, 1000.f);
	m_v3Camera_Lookat = glm::vec3(0.f, 0.f, 0.f);
	m_v3Camera_Up = glm::vec3(0.f, 1.f, 0.f);
	m_m4View = glm::lookAt(
		m_v3Camera_Position,
		m_v3Camera_Lookat,
		m_v3Camera_Up
	);

	//Initialize projection matrix
	m_m4OrthoProj = glm::ortho(
		-(float)windowSizeX / 2.f, (float)windowSizeX / 2.f,
		-(float)windowSizeY / 2.f, (float)windowSizeY / 2.f,
		0.0001f, 10000.f);
	m_m4PersProj = glm::perspectiveRH(45.f, 1.f, 1.f, 1000.f);

	//Initialize projection-view matrix
	m_m4ProjView = m_m4OrthoProj * m_m4View; //use ortho at this time
	//m_m4ProjView = m_m4PersProj * m_m4View;

	//Initialize model transform matrix :; used for rotating quad normal to parallel to camera direction
	m_m4Model = glm::rotate(glm::mat4(1.0f), glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
}

void Renderer::CreateVertexBufferObjects()
{
	float myTest[18] = { 0, };

	// 샘플1
	float rect[] =
	{
		-0.5, -0.5, 0.f, -0.5, 0.5, 0.f, 0.5, 0.5, 0.f, //Triangle1
		-0.5, -0.5, 0.f,  0.5, 0.5, 0.f, 0.5, -0.5, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	// 샘플2
	float lecture2[] =
	{
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
	}; // 9 floats array

	glGenBuffers(1, &m_VBOLecture2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture2), lecture2, GL_STATIC_DRAW);

	// 샘플3
	float lecture3[] =
	{
		0.0, 0.0, 0.0, 1, 0, 0, 1,
		1.0, 0.0, 0.0, 0, 1, 0, 1,
		1.0, 1.0, 0.0, 0, 0, 1, 1
	}; // 9 position floats array + 12 RGBA color float array = 21 float array

	glGenBuffers(1, &m_VBOLecture3);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3), lecture3, GL_STATIC_DRAW);

	// // 샘플3 - 파티클 생성 용도 - VBO준비 (lecture3_3 Study)
	float particleSize = 0.1f;
	float lecture3_singleParticle[] =
	{
		-particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1,
		-particleSize,  particleSize, 0.0, 1, 1, 1, 1,

		-particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1
	}; // 삼각형 2개를 붙인 쿼드

	glGenBuffers(1, &m_VBOSingleParticleQuad);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSingleParticleQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lecture3_singleParticle), lecture3_singleParticle, GL_STATIC_DRAW);
}

void Renderer::CreateParticle(int count)
{
	// count == 파티클의 형태[사각형(쿼드)]의 개수
	int floatCount = count * (3 + 3 + 1 + 1) * 3 * 2; //(x, y, z) 3vertex , (vx, vy , vz) 3Velocity, 1EmitTime, 1lifeTime, 2Triangle = 1 Quad
	float* particleVertices = new float[floatCount];
	int vertexCount = count * 3 * 2;

	int index = 0;
	float particleSize = 0.01f;

	for (int i = 0; i < count; i++)
	{
		float randomValueX = 0.f;
		float randomValueY = 0.f;
		float randomValueZ = 0.f;
		float randomValueVX = 0.f;
		float randomValueVY = 0.f;
		float randomValueVZ = 0.f;
		float randomEmitTime = 0.f;
		float randomLifeTime = 0.f;

		randomValueX = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueY = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueZ = 0.f;
		randomValueVX = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.f; //-1~1
		randomValueVZ = 0.f;
		randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.f;
		randomLifeTime = ((float)rand() / (float)RAND_MAX) * 0.5f;
		
		//v0
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		//v1
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		//v2
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		//v3
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		//v4
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		//v5
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime
	}
	glGenBuffers(1, &m_VBOManyParticle);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, particleVertices, GL_STATIC_DRAW);
	m_VBOManyParticleVertexCount = vertexCount;
	delete[] particleVertices;
}

void Renderer::CreateParticleAnimation5(int count)
{
	// count == 파티클의 형태[사각형(쿼드)]의 개수
	int floatCount = count * (3 + 3 + 1 + 1 + 1 + 1) * 3 * 2; // (x, y, z, vx, vy, vz, emit, life, amp, period)
	float* particleVertices = new float[floatCount];
	int vertexCount = count * 3 * 2;

	int index = 0;
	float particleSize = 0.01f;

	for (int i = 0; i < count; i++)
	{
		float randomValueX = 0.f;
		float randomValueY = 0.f;
		float randomValueZ = 0.f;
		float randomValueVX = 0.f;
		float randomValueVY = 0.f;
		float randomValueVZ = 0.f;
		float randomEmitTime = 0.f;
		float randomLifeTime = 0.f;
		float randomAmp = 0.f;
		float randomPeriod = 0.f;

		randomValueX = 0.f;
		randomValueY = 0.f;
		randomValueZ = 0.f;
		randomValueVX = 1.f;
		randomValueVY = 0.f;
		randomValueVZ = 0.f;
		randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.f;
		randomLifeTime = 1.f;
		randomAmp = ((float)rand() / (float)RAND_MAX) * 0.2f - 0.1f; // (-0.1 ~ 0.1)
		randomPeriod = ((float)rand() / (float)RAND_MAX) * 2.f; // (0~2)

		//v0
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period

		//v1
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period

		//v2
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period

		//v3
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period

		//v4
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period

		//v5
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
	}
	glGenBuffers(1, &m_VBOManyParticle1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, particleVertices, GL_STATIC_DRAW);
	m_VBOManyParticleVertexCount1 = vertexCount;
	delete[] particleVertices;
}

void Renderer::CreateParticleAnimation5_Transform(int count)
{
	// count == 파티클의 형태[사각형(쿼드)]의 개수
	int floatCount = count * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4) * 3 * 2; // (x, y, z, vx, vy, vz, emit, life, amp, period, randomValue, rgba)
	float* particleVertices = new float[floatCount];
	int vertexCount = count * 3 * 2;

	int index = 0;
	float particleSize = 0.01f;

	for (int i = 0; i < count; i++)
	{
		float randomValueX = 0.f;
		float randomValueY = 0.f;
		float randomValueZ = 0.f;
		float randomValueVX = 0.f;
		float randomValueVY = 0.f;
		float randomValueVZ = 0.f;
		float randomEmitTime = 0.f;
		float randomLifeTime = 0.f;
		float randomAmp = 0.f;
		float randomPeriod = 0.f;
		float randomValue = 0.f;
		float randomR, randomG, randomB, randomA = 0.f;

		randomValueX = 0.f;
		randomValueY = 0.f;
		randomValueZ = 0.f;
		randomValueVX = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;
		randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;
		randomValueVZ = 0.f;
		randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.f;
		randomLifeTime = ((float)rand() / (float)RAND_MAX) * 2.f;
		randomAmp = ((float)rand() / (float)RAND_MAX) * 0.2f - 0.1f; // (-0.1 ~ 0.1)
		randomPeriod = ((float)rand() / (float)RAND_MAX) * 2.f; // (0~2)
		randomValue = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomR = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomG = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomB = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomA = 1.f;

		//v0
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v1
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v2
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v3
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v4
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v5
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha
	}
	glGenBuffers(1, &m_VBOManyParticle2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, particleVertices, GL_STATIC_DRAW);
	m_VBOManyParticleVertexCount2 = vertexCount;
	delete[] particleVertices;
}

void Renderer::CreateVertexBufferObjectsLecture4_1()
{
	// 화면에 커다란 사각형 그리기(색은 흰색으로) Lecture 4_1
	// Min(-0.5, -0.5), Max(0.5, 0.5)
	float particleSize = 0.5f;
	float singleParticleLecture4_1[] =
	{
		-particleSize, -particleSize, 0.0, 1, 1, 1, 1, // x, y, z, r, g, b, a --> AttribPointer
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1,
		-particleSize,  particleSize, 0.0, 1, 1, 1, 1,

		-particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1
	}; // 삼각형 2개를 붙인 쿼드

	glGenBuffers(1, &m_VBOSingleParticleQuadLecture4_1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSingleParticleQuadLecture4_1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(singleParticleLecture4_1), singleParticleLecture4_1, GL_STATIC_DRAW);
}

void Renderer::CreateParticleAnimation_Lecture4_2(int count)
{
	// count == 파티클의 형태[사각형(쿼드)]의 개수
	int floatCount = count * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4) * 3 * 2; // (x, y, z, vx, vy, vz, emit, life, amp, period, randomValue, rgba)
	float* particleVertices = new float[floatCount];
	int vertexCount = count * 3 * 2;

	int index = 0;
	float particleSize = 0.1f;

	for (int i = 0; i < count; i++)
	{
		float randomValueX = 0.f;
		float randomValueY = 0.f;
		float randomValueZ = 0.f;
		float randomValueVX = 0.f;
		float randomValueVY = 0.f;
		float randomValueVZ = 0.f;
		float randomEmitTime = 0.f;
		float randomLifeTime = 0.f;
		float randomAmp = 0.f;
		float randomPeriod = 0.f;
		float randomValue = 0.f;
		float randomR, randomG, randomB, randomA = 0.f;

		randomValueX = 0.f;
		randomValueY = 0.f;
		randomValueZ = 0.f;
		randomValueVX = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;
		randomValueVY = ((float)rand() / (float)RAND_MAX - 0.5f) * 0.1f;
		randomValueVZ = 0.f;
		randomEmitTime = ((float)rand() / (float)RAND_MAX) * 5.f;
		randomLifeTime = ((float)rand() / (float)RAND_MAX) * 2.f;
		randomAmp = ((float)rand() / (float)RAND_MAX) * 0.2f - 0.1f; // (-0.1 ~ 0.1)
		randomPeriod = ((float)rand() / (float)RAND_MAX) * 2.f; // (0~2)
		randomValue = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomR = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomG = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomB = ((float)rand() / (float)RAND_MAX) * 1.f; // (0~1)
		randomA = 1.f;

		//v0
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v1
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v2
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v3
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = -particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v4
		particleVertices[index] = particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha

		//v5
		particleVertices[index] = -particleSize / 2.f + randomValueX;
		index++;
		particleVertices[index] = particleSize / 2.f + randomValueY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Position XYZ

		particleVertices[index] = randomValueVX;
		index++;
		particleVertices[index] = randomValueVY;
		index++;
		particleVertices[index] = 0.f;
		index++; //Velocity XYZ

		particleVertices[index] = randomEmitTime;
		index++; //EmitTime
		particleVertices[index] = randomLifeTime;
		index++; //LifeTime

		particleVertices[index] = randomAmp;
		index++; //Amp
		particleVertices[index] = randomPeriod;
		index++; //Period
		particleVertices[index] = randomValue;
		index++; //Random Value

		// Color
		particleVertices[index] = randomR;
		index++; //Random Color Red
		particleVertices[index] = randomG;
		index++; //Random Color Green
		particleVertices[index] = randomB;
		index++; //Random Color Blue
		particleVertices[index] = randomA;
		index++; //Random Color Alpha
	}
	glGenBuffers(1, &m_VBOManyParticle_Lecture4_2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, particleVertices, GL_STATIC_DRAW);
	m_VBOManyParticleVertexCount_Lecture4_2 = vertexCount;
	delete[] particleVertices;
}

void Renderer::CreateParticleAnimation_Lecture4_3(int count)
{
	// 화면에 커다란 사각형 그리기(색은 흰색으로) Lecture 4_1
	// Min(-0.5, -0.5), Max(0.5, 0.5)
	float particleSize = 0.5f;
	float singleParticleVertexCountLecture4_3[] =
	{
		-particleSize, -particleSize, 0.0, 1, 1, 1, 1, // x, y, z, r, g, b, a --> AttribPointer
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1,
		-particleSize,  particleSize, 0.0, 1, 1, 1, 1,

		-particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize, -particleSize, 0.0, 1, 1, 1, 1,
		 particleSize,  particleSize, 0.0, 1, 1, 1, 1
	}; // 삼각형 2개를 붙인 쿼드

	glGenBuffers(1, &m_VBOManyParticle_Lecture4_3);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(singleParticleVertexCountLecture4_3), singleParticleVertexCountLecture4_3, GL_STATIC_DRAW);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string* target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char* Renderer::loadBMPRaw(const char* imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char* data;

	// Open the file
	FILE* file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char* filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char* filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char* bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

void Renderer::Test() // 렌더링 예시 1 (그리는 부분)
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture2() // 렌더링 예시 2 (그리는 부분)
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::MyTest() // 렌더링 예시 3 (그리는 부분)
{
	glUseProgram(m_SolidRectShader);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture2); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(attribPosition);
}

float gTime = 1.f; // 시간에 따른 컬러값 변화를 주기 위함.

void Renderer::Lecture3() // 문제가 있네
{
	GLuint shader = m_Lecture3Shader; // 쉐이더가 1개이면 깔끔하게 shader라는 변수를 만들어서 사용하는 것이 보기에 편할 것.
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0); // 좌표 (x,y,z) 총 3개

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOLecture3); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLuint*)(sizeof(float) * 3)); // 앞에 좌표 총 3개 뒤에 시작해야 한다.

	// 시간이라는 외부 입력 (변화를) 쉐이더를 통해 전달했다. (vertex shader에 전달)
	int uniformLocTime = glGetUniformLocation(shader, "u_Time"); // 보통 unsigned int로 쓰기도 하는데 int로 설정하면 오류났을 때 -1값으로 나오기 때문에 int형으로 사용하는 것 또한 추천 // 안쓰고 있으면 -1값(오류)이 발생
	glUniform1f(uniformLocTime, gTime); // 런타임 중 유니폼으로 넘긴 값이 -1으로 되어있으면 외부 입력이 실패했다는 이야기이다.

	int unifromLocColor = glGetUniformLocation(shader, "u_Color"); // 색상 넣어주기
	glUniform4f(unifromLocColor, 1, 1, 1, 1); // RGBA(1,1,1,1)

	glDrawArrays(GL_TRIANGLES, 0, 3);

	gTime -= 0.01f;
	if (gTime < 0.f)
	{
		gTime = 1.f;
	}

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture3_Particle()
{
	GLuint shader = m_Lecture3ParticleShader; // 쉐이더가 1개이면 깔끔하게 shader라는 변수를 만들어서 사용하는 것이 보기에 편할 것.
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); // 좌표 (x,y,z) 총 3개 // 2번째 인자의 크기 3과 같다면 sizeof(float) * 3이 아닌 0으로 설정해도 문제없이 작동한다.

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLuint*)(sizeof(float) * 3));

	int attribEmitTime = glGetAttribLocation(shader, "a_EmitTime");
	glEnableVertexAttribArray(attribEmitTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);
	glVertexAttribPointer(attribEmitTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLuint*)(sizeof(float) * 6));

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLuint*)(sizeof(float) * 7));


	// 시간이라는 외부 입력 (변화를) 쉐이더를 통해 전달했다. (vertex shader에 전달)
	int uniformLocTime = glGetUniformLocation(shader, "u_Time"); // 보통 unsigned int로 쓰기도 하는데 int로 설정하면 오류났을 때 -1값으로 나오기 때문에 int형으로 사용하는 것 또한 추천 // 안쓰고 있으면 -1값(오류)이 발생
	glUniform1f(uniformLocTime, gTime); // 런타임 중 유니폼으로 넘긴 값이 -1으로 되어있으면 외부 입력이 실패했다는 이야기이다.

	int uniformAccel = glGetUniformLocation(shader, "u_Accel");
	glUniform3f(uniformAccel, std::sin(gTime), std::cos(gTime), 0.0);


	glDrawArrays(GL_TRIANGLES, 0, m_VBOManyParticleVertexCount); // 3개 * 2 = 정점 3개를 하나의 삼각형으로 그리고 이를 삼각형 2개로 이어 만들어본다.
	
	float addTime = 0.001f;
	gTime += addTime;

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture3_6ParticleAnimation5()
{
	GLuint shader = m_Lecture3ParticleShader1; // 쉐이더가 1개이면 깔끔하게 shader라는 변수를 만들어서 사용하는 것이 보기에 편할 것.
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle1); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0); // 좌표 (x,y,z) 총 3개 // 2번째 인자의 크기 3과 같다면 sizeof(float) * 3이 아닌 0으로 설정해도 문제없이 작동한다.

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle1); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLuint*)(sizeof(float) * 3));

	int attribEmitTime = glGetAttribLocation(shader, "a_EmitTime");
	glEnableVertexAttribArray(attribEmitTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle1);
	glVertexAttribPointer(attribEmitTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLuint*)(sizeof(float) * 6));

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle1);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLuint*)(sizeof(float) * 7));

	int attribAmpTime = glGetAttribLocation(shader, "a_AmpTime");
	glEnableVertexAttribArray(attribAmpTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle1);
	glVertexAttribPointer(attribAmpTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLuint*)(sizeof(float) * 8));

	int attribPeriodTime = glGetAttribLocation(shader, "a_PeriodTime");
	glEnableVertexAttribArray(attribPeriodTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle1);
	glVertexAttribPointer(attribPeriodTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLuint*)(sizeof(float) * 9));


	// 시간이라는 외부 입력 (변화를) 쉐이더를 통해 전달했다. (vertex shader에 전달)
	int uniformLocTime = glGetUniformLocation(shader, "u_Time"); // 보통 unsigned int로 쓰기도 하는데 int로 설정하면 오류났을 때 -1값으로 나오기 때문에 int형으로 사용하는 것 또한 추천 // 안쓰고 있으면 -1값(오류)이 발생
	glUniform1f(uniformLocTime, gTime); // 런타임 중 유니폼으로 넘긴 값이 -1으로 되어있으면 외부 입력이 실패했다는 이야기이다.

	int uniformAccel = glGetUniformLocation(shader, "u_Accel");
	glUniform3f(uniformAccel, 0.0, 0.0, 0.0);


	glDrawArrays(GL_TRIANGLES, 0, m_VBOManyParticleVertexCount1); // 3개 * 2 = 정점 3개를 하나의 삼각형으로 그리고 이를 삼각형 2개로 이어 만들어본다.

	float addTime = 0.01f;
	gTime += addTime;

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture3_7ParticleAnimation5_Transform()
{
	GLuint shader = m_Lecture3ParticleShader2; // 쉐이더가 1개이면 깔끔하게 shader라는 변수를 만들어서 사용하는 것이 보기에 편할 것.
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0); // 좌표 (x,y,z) 총 3개 // 2번째 인자의 크기 3과 같다면 sizeof(float) * 3이 아닌 0으로 설정해도 문제없이 작동한다.

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 3));

	int attribEmitTime = glGetAttribLocation(shader, "a_EmitTime");
	glEnableVertexAttribArray(attribEmitTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2);
	glVertexAttribPointer(attribEmitTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 6));

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 7));

	int attribAmpTime = glGetAttribLocation(shader, "a_AmpTime");
	glEnableVertexAttribArray(attribAmpTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2);
	glVertexAttribPointer(attribAmpTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 8));

	int attribPeriodTime = glGetAttribLocation(shader, "a_PeriodTime");
	glEnableVertexAttribArray(attribPeriodTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2);
	glVertexAttribPointer(attribPeriodTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 9));

	int attribRandomValue = glGetAttribLocation(shader, "a_RandomValue");
	glEnableVertexAttribArray(attribRandomValue);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2);
	glVertexAttribPointer(attribRandomValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 10));

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle2);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 11));


	// 시간이라는 외부 입력 (변화를) 쉐이더를 통해 전달했다. (vertex shader에 전달)
	int uniformLocTime = glGetUniformLocation(shader, "u_Time"); // 보통 unsigned int로 쓰기도 하는데 int로 설정하면 오류났을 때 -1값으로 나오기 때문에 int형으로 사용하는 것 또한 추천 // 안쓰고 있으면 -1값(오류)이 발생
	glUniform1f(uniformLocTime, gTime); // 런타임 중 유니폼으로 넘긴 값이 -1으로 되어있으면 외부 입력이 실패했다는 이야기이다.

	int uniformAccel = glGetUniformLocation(shader, "u_Accel");
	glUniform3f(uniformAccel, 0.f, 0.f, 0.f);


	glDrawArrays(GL_TRIANGLES, 0, m_VBOManyParticleVertexCount2); // 3개 * 2 = 정점 3개를 하나의 삼각형으로 그리고 이를 삼각형 2개로 이어 만들어본다.

	float addTime = 0.01f;
	gTime += addTime;

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture4_1()
{
	GLuint shader = m_ParticleShaderLectrue4_1;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);	
	// 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSingleParticleQuadLecture4_1);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOSingleParticleQuadLecture4_1);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLuint*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6); // 총 6개의 정점(사각형 쿼드 1개)을 그린다.

	glDisableVertexAttribArray(attribPosition);
}

void Renderer::Lecture4_2()
{
	GLuint shader = m_ShaderParticle_Lecture4_2; // 쉐이더가 1개이면 깔끔하게 shader라는 변수를 만들어서 사용하는 것이 보기에 편할 것.
	glUseProgram(shader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0); // 좌표 (x,y,z) 총 3개 // 2번째 인자의 크기 3과 같다면 sizeof(float) * 3이 아닌 0으로 설정해도 문제없이 작동한다.

	int attribVelocity = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(attribVelocity);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2); // 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glVertexAttribPointer(attribVelocity, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 3));

	int attribEmitTime = glGetAttribLocation(shader, "a_EmitTime");
	glEnableVertexAttribArray(attribEmitTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2);
	glVertexAttribPointer(attribEmitTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 6));

	int attribLifeTime = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(attribLifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2);
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 7));

	int attribAmpTime = glGetAttribLocation(shader, "a_AmpTime");
	glEnableVertexAttribArray(attribAmpTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2);
	glVertexAttribPointer(attribAmpTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 8));

	int attribPeriodTime = glGetAttribLocation(shader, "a_PeriodTime");
	glEnableVertexAttribArray(attribPeriodTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2);
	glVertexAttribPointer(attribPeriodTime, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 9));

	int attribRandomValue = glGetAttribLocation(shader, "a_RandomValue");
	glEnableVertexAttribArray(attribRandomValue);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2);
	glVertexAttribPointer(attribRandomValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 10));

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_2);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLuint*)(sizeof(float) * 11));


	// 시간이라는 외부 입력 (변화를) 쉐이더를 통해 전달했다. (vertex shader에 전달)
	int uniformLocTime = glGetUniformLocation(shader, "u_Time"); // 보통 unsigned int로 쓰기도 하는데 int로 설정하면 오류났을 때 -1값으로 나오기 때문에 int형으로 사용하는 것 또한 추천 // 안쓰고 있으면 -1값(오류)이 발생
	glUniform1f(uniformLocTime, gTime); // 런타임 중 유니폼으로 넘긴 값이 -1으로 되어있으면 외부 입력이 실패했다는 이야기이다.

	int uniformAccel = glGetUniformLocation(shader, "u_Accel");
	glUniform3f(uniformAccel, 0.f, 0.f, 0.f);


	glDrawArrays(GL_TRIANGLES, 0, m_VBOManyParticleVertexCount_Lecture4_2); // 3개 * 2 = 정점 3개를 하나의 삼각형으로 그리고 이를 삼각형 2개로 이어 만들어본다.

	float addTime = 0.01f;
	gTime += addTime;

	glDisableVertexAttribArray(attribPosition);
	glDisable(GL_BLEND);
}

//  (-0.5 ~ 0.5)
float g_points[] = {
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f,
	(float)((float)rand() / (float)RAND_MAX), (float)((float)rand() / (float)RAND_MAX), 0.01f
};


float g_startTime = 0.f;
void Renderer::Lecture4_3()
{
	GLuint shader = m_ShaderParticle_Lecture4_3;
	glUseProgram(shader);

	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	// 굳이 안해줘도 될 부분. 마지막으로 바인드가 되기는 했을테니... 하지만 혹시나 꼬였을 가능성이 있기 때문에 한번 더 해준 것이다.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_3);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);

	int attribColor = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(attribColor);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOManyParticle_Lecture4_3);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (GLuint*)(sizeof(float) * 3));

	int uniformPoints = glGetUniformLocation(shader, "u_Points");
	glUniform3fv(uniformPoints, 10, g_points); // v: array형태로 넘기겠다.

	int uniformStartTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uniformStartTime, g_startTime); // v: array형태로 넘기겠다.

	g_startTime += 0.001;

	glDrawArrays(GL_TRIANGLES, 0, 6); // 총 6개의 정점(사각형 쿼드 1개)을 그린다.

	glDisableVertexAttribArray(attribPosition);
}