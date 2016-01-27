#pragma once
#define BLOCK_SIZE 32
#define BLOCK_SIZEF (float)32
#include <GL\glew.h>
#include <vector>
#include "Settings.h"

class Drawer
{
public:
	GLuint block_vbo;
	GLuint screen_vbo;

	GLuint block_vao;
	GLuint screen_vao;

	GLuint current_vao;


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

		glm::vec2 screen_vertices[12] = {
			glm::vec2(0, 0), glm::vec2(0.0f, 0.0f),
			glm::vec2(0, 1.0f), glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f),
			glm::vec2(0, 0), glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0), glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)
		};

		glGenVertexArrays(1, &block_vao);
		glBindVertexArray(block_vao);

		glGenBuffers(1, &block_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, block_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*) 8);
		glBindVertexArray(0);

		glGenVertexArrays(1, &screen_vao);
		glBindVertexArray(screen_vao);

		glGenBuffers(1, &screen_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertices), screen_vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
		glBindVertexArray(0);
	}

	void DrawRectangle(double x, double y, double width, double height)
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Settings::DRAW_CALLS++;
	}

	void Draw(GLuint vao)
	{
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Settings::DRAW_CALLS++;
		glBindVertexArray(0);
	}

	void DrawBlock()
	{
		Draw(block_vao);
	}

	void DrawScreen()
	{
		Draw(screen_vao);
	}

	int next_p2(int a)
	{
		int rval = 2;
		while (rval<a)
			rval <<= 1;
		return rval;
	}

};

