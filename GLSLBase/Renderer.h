#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void Lecture2();
	void MyTest();
	void Lecture3();
	void Lecture3_Particle();
	void Lecture3_6ParticleAnimation5();
	void Lecture3_7ParticleAnimation5_Transform();
	void Lecture4_1();
	void Lecture4_2();
	void Lecture4_3();
	void Lecture4_4();
	void Lecture4_5();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);
	void CreateParticle(int count);
	void CreateParticleAnimation5(int count);
	void CreateParticleAnimation5_Transform(int count);
	void CreateVertexBufferObjectsLecture4_1();
	void CreateParticleAnimation_Lecture4_2(int count);
	void CreateParticleAnimation_Lecture4_3(int count);
	void CreateParticleAnimation_Lecture4_4(int count);
	void CreateParticleAnimation_Lecture4_5(int count);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	//camera position
	glm::vec3 m_v3Camera_Position;
	//camera lookat position
	glm::vec3 m_v3Camera_Lookat;
	//camera up vector
	glm::vec3 m_v3Camera_Up;

	glm::mat4 m_m4OrthoProj;
	glm::mat4 m_m4PersProj;
	glm::mat4 m_m4Model;
	glm::mat4 m_m4View;
	glm::mat4 m_m4ProjView;

	GLuint m_VBORect = 0;
	GLuint m_SolidRectShader = 0;

	// 샘플2 VBO
	GLuint m_VBOLecture2 = 0;

	// 복사한 쉐이더 파일과 샘플3 제작을 위한 VBO를 추가한다.
	GLuint m_Lecture3Shader = 0;
	GLuint m_VBOLecture3 = 0;

	// 쿼드로 만들 파티클 VertexBufferObject
	GLuint m_VBOSingleParticleQuad = 0;
	GLuint m_Lecture3ParticleShader = 0;

	GLuint m_VBOManyParticle = 0;
	GLuint m_VBOManyParticleVertexCount = 0;

	GLuint m_VBOManyParticle1 = 0;
	GLuint m_VBOManyParticleVertexCount1 = 0;

	GLuint m_VBOManyParticle2 = 0;
	GLuint m_VBOManyParticleVertexCount2 = 0;

	// 쿼드로 만들 파티클 VertexBufferObject
	GLuint m_VBOSingleParticleQuad1 = 0;
	GLuint m_Lecture3ParticleShader1 = 0;
	GLuint m_Lecture3ParticleShader2 = 0;

	// 화면에 커다란 사각형 그리기 (색은 흰색으로) Lecture4_0
	GLuint m_VBOSingleParticleQuadLecture4_1 = 0;
	GLuint m_ParticleShaderLectrue4_1 = 0;

	// Lecture4_2
	GLuint m_VBOManyParticle_Lecture4_2 = 0;
	GLuint m_ShaderParticle_Lecture4_2 = 0;
	GLuint m_VBOManyParticleVertexCount_Lecture4_2 = 0;

	// Lecture4_3
	GLuint m_VBOManyParticle_Lecture4_3 = 0;
	GLuint m_ShaderParticle_Lecture4_3 = 0;
	GLuint m_VBOManyParticleVertexCount_Lecture4_3 = 0;

	// Lecture4_4
	GLuint m_VBOManyParticle_Lecture4_4 = 0;
	GLuint m_ShaderParticle_Lecture4_4 = 0;
	GLuint m_VBOManyParticleVertexCount_Lecture4_4 = 0;

	// Lecture4_4
	GLuint m_VBOManyParticle_Lecture4_5 = 0;
	GLuint m_ShaderParticle_Lecture4_5 = 0;
	GLuint m_VBOManyParticleVertexCount_Lecture4_5 = 0;
	// Lecture4_5 - Vertex Shader Input Data Packing
	GLuint m_VBOPack0_Pos = 0;
	GLuint m_VBOPack0_Color = 0;
	GLuint m_VBOPack1 = 0;
};