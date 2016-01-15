#pragma once
#include "ResourceManager.h"
#include "Inventory.h"

#include "Chat.h"

class HUD
{
public:
	int ScreenWidth;
	int ScreenHeight;

	GLuint mVbo;
	bool inventoryOpen = false;
	glm::vec3 slotColor = glm::vec3(0, 0.1, 0.2);
	glm::vec3 selectedColor = glm::vec3(0, 0.2, 0.4);

	Chat* chat;
	GLFWwindow* window;

	HUD(GLFWwindow* window)
	{
		this->window = window;

		glm::vec2 vertices[12] = {
			glm::vec2(0, 0), glm::vec2(0, 0),
			glm::vec2(0, 1), glm::vec2(0, 1),
			glm::vec2(1, 1), glm::vec2(1, 1),
			glm::vec2(0, 0), glm::vec2(0, 0),
			glm::vec2(1, 0), glm::vec2(1, 0),
			glm::vec2(1, 1), glm::vec2(1, 1)
		};

		glGenBuffers(1, &mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*) 8);

		chat = new Chat();
		chat->Say("Chat has been enabled.");


		glfwGetWindowSize(window, &ScreenWidth, &ScreenHeight);
	}

	void Render(glm::mat4 mvp, Inventory* inventory, int selectedSlot, bool inventoryOpen)//, Printer* printer)
	{
		ResourceManager* resources = ResourceManager::GetInstance();
		Shader* framedShader = resources->GetShader("framed");
		framedShader->Bind();
		GLuint framed_posLoc = framedShader->GetUniformLocation("pos");
		GLuint framed_mvpLoc = framedShader->GetUniformLocation("MVP");
		GLuint framed_sizeLoc = framedShader->GetUniformLocation("size");
		GLuint framed_frameLoc = framedShader->GetUniformLocation("frame");
		GLuint framed_lineCountLoc = framedShader->GetUniformLocation("lineCount");
		GLuint framed_dirLoc = framedShader->GetUniformLocation("dir");
		GLuint framed_textureLoc = framedShader->GetUniformLocation("frameTexture");
		GLuint framed_totalFramesLoc = framedShader->GetUniformLocation("totalFrames");
		GLuint framed_alphaLoc = framedShader->GetUniformLocation("alphaValue");
		GLuint framed_lightLoc = framedShader->GetUniformLocation("light");

		glActiveTexture(GL_TEXTURE0);
		glUniform1f(framed_textureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, resources->GetTexture("slot"));

		glUniformMatrix4fv(framed_mvpLoc, 1, false, &mvp[0][0]);

		int slotSize = 42;
		int space = 1;
		int winX = ScreenWidth / 2 - (slotSize + space) * 5;
		glUniform1f(framed_totalFramesLoc, 1);
		glUniform1f(framed_lineCountLoc, 1);
		glUniform1f(framed_frameLoc, 0);
		glUniform1f(framed_alphaLoc, 1);
		glUniform1f(framed_lightLoc, 1);
		glUniform2f(framed_sizeLoc, slotSize / (float) BLOCK_SIZE, slotSize / (float) BLOCK_SIZE);
		//glUniform3f(tintLoc, slotColor.r, slotColor.g, slotColor.b);

		for (int i = 0; i < (inventoryOpen ? inventory->Size : 10); i++)
		{
			if (i == selectedSlot)
				continue;
			glUniform2f(framed_posLoc, winX + (i % 10) * (slotSize + space), (i / 10) * (slotSize + space));
			RenderSlot();
		}
		//glUniform3f(tintLoc, selectedColor.r, selectedColor.g, selectedColor.b);
		glUniform2f(framed_sizeLoc, 46 / (float) BLOCK_SIZE, 46 / (float) BLOCK_SIZE);
		glUniform2f(framed_posLoc, winX + (selectedSlot % 10) * (slotSize + space) - 2, (selectedSlot / 10) * (slotSize + space) - 2);
		RenderSlot();

		std::shared_ptr<Item> slotItem;
		std::shared_ptr<ItemBlock> slotItemBlock;
		//glUniform3f(tintLoc, 0, 0, 0);
		glUniform2f(framed_sizeLoc, 48 / (float) BLOCK_SIZE, 48 / (float) BLOCK_SIZE);
		glUniform1f(framed_totalFramesLoc, 16);
		glUniform1f(framed_lineCountLoc, 16);
		glUniform1f(framed_dirLoc, 1);
		glBindTexture(GL_TEXTURE_2D, resources->GetTexture("blocks"));
		for (int i = 0; i < (inventoryOpen ? inventory->Size : 10); i++)
		{
			slotItem = inventory->GetItem(i);
			if (slotItem == nullptr || !slotItem->block)
				continue;
			slotItemBlock = std::dynamic_pointer_cast<ItemBlock>(slotItem);
			Block* block = Block::GetBlockById(slotItemBlock->blockId);
			if (block == nullptr)
				continue;
			int index = block->GetTextureCoord();
			glUniform2f(framed_posLoc, winX + (i % 10) * (slotSize + space) + slotSize / 2 - 24, (i / 10) * (slotSize + space) + slotSize / 2 - 24);
			glUniform1f(framed_frameLoc, index);
			RenderSlot();
		}
		glUniform2f(framed_sizeLoc, 32 / (float) BLOCK_SIZE, 32 / (float) BLOCK_SIZE);
		glBindTexture(GL_TEXTURE_2D, resources->GetTexture("Items"));
		for (int i = 0; i < (inventoryOpen ? inventory->Size : 10); i++)
		{
			slotItem = inventory->GetItem(i);
			if (slotItem == nullptr || slotItem->block)
				continue;
			int index = slotItem->textureIndex;
			glUniform2f(framed_posLoc, winX + (i % 10) * (slotSize + space) + slotSize / 2 - 24 + 6, (i / 10) * (slotSize + space) + slotSize / 2 - 24 + 6);
			glUniform1f(framed_frameLoc, index);
			RenderSlot();
		}
		framedShader->Release();
		
		framedShader->Bind();
		glBindTexture(GL_TEXTURE_2D, resources->GetTexture("cursor"));
		glUniform1f(framed_totalFramesLoc, 1);
		glUniform1f(framed_lineCountLoc, 1);
		glUniform1f(framed_frameLoc, 0);
		glUniform1f(framed_alphaLoc, 1);
		glUniform1f(framed_lightLoc, 1);
		glUniform2f(framed_sizeLoc, 32 / (float) BLOCK_SIZE, 32 / (float) BLOCK_SIZE);

		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glUniform2f(framed_posLoc, x, y);
		RenderSlot();

		framedShader->Release();

		Shader* fontShader = resources->GetShader("font");
		fontShader->Bind();

		glUniformMatrix4fv(fontShader->GetUniformLocation("MVP"), 1, false, &mvp[0][0]);
		glUniform1f(fontShader->GetUniformLocation("tex"), 0);
		glUniform4f(fontShader->GetUniformLocation("color"), 1, 1, 1, 1);
		glActiveTexture(GL_TEXTURE0);

		for (int i = 0; i < (inventoryOpen ? inventory->Size : 10); i++)
		{
			slotItem = inventory->GetItem(i);
			if (slotItem == nullptr)
				continue;
			int index = slotItem->Id;
			float posx = winX + (i % 10) * (slotSize + space) + slotSize / 2 - 24;
			float posy = (i / 10) * (slotSize + space) + slotSize / 2 - 24;
			//printer->render_text(std::to_string(slotItem->stack).c_str(), posx + 8, posy + slotSize - 16, 14);
		}

		glUniform4f(fontShader->GetUniformLocation("color"), 0, 0, 0, 1);
		/*printer->render_text(std::string("FPS: ").append(std::to_string(Settings::FPS)).c_str(), 0, 70);
		printer->render_text(std::string("Ping: ").append(std::to_string(Settings::Ping)).append(" ms.").c_str(), 0, 100);
		printer->render_text(std::string("RAM: ").append(std::to_string(Settings::RamUsage / 1000000)).append(" MB.").c_str(), 0, 130);*/

		//Render Chat
		glm::vec3 color = glm::vec3();
		for (int i = 9; i >= 0; i--)
		{
			if (chat->linesColor[i] != color) {
				color = chat->linesColor[i];
				glUniform4f(fontShader->GetUniformLocation("color"), color.r, color.g, color.b, 1);
			}
			//printer->render_text(chat->lines[i].c_str(), 0, 130 + (9 - i) * 20, 20);
		}
		glUniform4f(fontShader->GetUniformLocation("color"), 0, 0, 0, 1);
		if (chat->open)
			//printer->render_text(chat->newLine.c_str(), 0, 130 + 10 * 20, 20);

		//Render Cursor
		if (chat->open && clock() / 500 % 2 == 0) {
			std::string s;
			s.append(chat->newLine.c_str(), chat->pointer);
			//printer->render_text("|", printer->measure_text(s.c_str(), 20) - (s.size() > 0 ? 3 : 0), 130 + 10 * 20, 20);
		}

		fontShader->Release();
	}

	void RenderSlot()
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	std::string GetTimeString(double time)
	{
		std::string str;
		std::string hours = std::to_string((int) *Settings::Time);
		std::string mins = std::to_string((int) ((*Settings::Time - (int) *Settings::Time) * 60));
		str.append((hours.size() > 1 ? "" : "0")).append(hours).append(":").append((mins.size() > 1 ? "" : "0")).append(mins);
		return str;
	}
};

