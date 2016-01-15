#pragma once
#define BLOCK_SIZE 32
#define BLOCK_SIZEF (float)32
#include <GL\glew.h>
#include <vector>
#include "Settings.h"

class Drawer
{
public:
	GLuint mVbo;
	Drawer()
	{
		const float halfpixel = 0.5f / 48.0f;

		glm::vec2 vertices[12] = {
			glm::vec2(0, 0), glm::vec2(halfpixel, halfpixel),
			glm::vec2(0, BLOCK_SIZE), glm::vec2(halfpixel, 1 - halfpixel),
			glm::vec2(BLOCK_SIZE, BLOCK_SIZE), glm::vec2(1 - halfpixel, 1 - halfpixel),
			glm::vec2(0, 0), glm::vec2(halfpixel, halfpixel),
			glm::vec2(BLOCK_SIZE, 0), glm::vec2(1 - halfpixel, halfpixel),
			glm::vec2(BLOCK_SIZE, BLOCK_SIZE), glm::vec2(1 - halfpixel, 1 - halfpixel)
		};

		glGenBuffers(1, &mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*) 8);
	}

	void DrawRectangle(double x, double y, double width, double height)
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Settings::DRAW_CALLS++;
	}

	void DrawBlock()
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Settings::DRAW_CALLS++;
	}

	void Draw()
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Settings::DRAW_CALLS++;
	}

	void DrawScreen()
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Settings::DRAW_CALLS++;
	}

	int next_p2(int a)
	{
		int rval = 2;
		while (rval<a)
			rval <<= 1;
		return rval;
	}

};

