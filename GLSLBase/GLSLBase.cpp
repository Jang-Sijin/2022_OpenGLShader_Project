/*
Copyright 2018 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.

Copyright 2022 Jang Sijin (Tech University of Korea)
Based on the shader sample, Jang Sijin added and modified the shader.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"

Renderer *g_Renderer = NULL;

int g_WindowSizeX = 500;
int g_WindowSizeY = 500;

// 프레임 레이트가 없으면 계속 호출된다.
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Renderer Test - maybe 60hz call
	// g_Renderer->Test(); // Sample1
	// g_Renderer->Lecture2(); // My Sample1
	// g_Renderer->Lecture3(); // Lecture3 study
	// g_Renderer->Lecture3_Particle(); // Lecture3-3 Particle test
	// g_Renderer->Lecture3_6ParticleAnimation5();
	// g_Renderer->Lecture3_7ParticleAnimation5_Transform();
	// g_Renderer->Lecture4_1();
	// g_Renderer->Lecture4_2();
	g_Renderer->Lecture4_3();
	
	// double buffers
	glutSwapBuffers();
}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	RenderScene();
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutCreateWindow("GLSL KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_4_6"))
	{
		std::cout << " GL Version is 4.6\n ";
	}
	else
	{
		std::cout << "GLEW 4.6 not supported\n ";
	}

	// Initialize Renderer
	g_Renderer = new Renderer(g_WindowSizeX, g_WindowSizeY);

	// 이벤트에 따른 콜백 함수를 호출한다.
	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyInput);

	glutMainLoop(); // 루프를 돈다.

	delete g_Renderer;

    return 0;
}

