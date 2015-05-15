#pragma once
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class FrameBufferObject
{
public:
	GLuint ID;
	GLuint textureID;
	int width, height;

	FrameBufferObject(const int width, const int height)
	{
		this->width = width;
		this->height = height;
		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);

		// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);


		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			printf("Frame Buffer Error");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	~FrameBufferObject()
	{
	}

	void Bind() {
		if (width != 1280 && height != 720)
			glViewport(0, 0, width, height);

		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}

	void Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (width != 1280 && height != 720)
			glViewport(0, 0, 1280, 720);
	}
};

