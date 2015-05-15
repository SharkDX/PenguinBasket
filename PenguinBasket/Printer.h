#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

class Printer
{
private:
	FT_Library library;
	FT_Face face;
	GLuint letters;
public:
	GLuint tex;
	GLuint vbo;
	Printer()
	{
		if (FT_Init_FreeType(&library))
			std::cout << "Free Type init error." << std::endl;
		FT_New_Face(library, "C:/Windows/Fonts/comicbd.ttf", 0, &face);
		FT_Set_Pixel_Sizes(face, 0, 48);

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenBuffers(1, &vbo);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	int next_p2(int a)
	{
		int rval = 2;
		while (rval<a)
			rval <<= 1;
		return rval;
	}

	void render_text(const char *text, float x, float y, int fontSize = 28) {
		float penx = 0;
		float peny = -fontSize;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindTexture(GL_TEXTURE_2D, tex);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		const char *p;
		FT_Set_Pixel_Sizes(face, 0, fontSize);
		
		for (p = text; *p; p++) {
			if (*p == '\n')
			{
				peny -= fontSize;
				penx = 0;
				continue;
			}
			if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
				continue;
			FT_GlyphSlot g = face->glyph;
			
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_ALPHA,
				g->bitmap.width,
				g->bitmap.rows,
				0,
				GL_ALPHA,
				GL_UNSIGNED_BYTE,
				g->bitmap.buffer
				);
			
			float x2 = penx + x + g->bitmap_left;
			float y2 = peny -y + g->bitmap_top;

			int w = (g->bitmap.width);
			int h = (g->bitmap.rows);

			GLfloat box[4][4] = {
				{ x2, -y2, 0, 0 },
				{ x2 + w, -y2, 1, 0 },
				{ x2, h - y2, 0, 1 },
				{ x2 + w, h - y2, 1, 1 },
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			penx += (g->advance.x >> 6);
			peny += (g->advance.y >> 6);
		}

		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	int measure_text(const char *text, int fontSize = 28)
	{
		int pen = 0;
		const char *p;
		FT_Set_Pixel_Sizes(face, 0, fontSize);

		for (p = text; *p; p++) {
			if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
				continue;
			pen += (face->glyph->advance.x >> 6);
		}
		return pen;
	}
};