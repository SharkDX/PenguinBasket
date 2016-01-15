#pragma once
#include <iterator>
typedef void(*CommandCallback)(std::string command, std::vector<std::string> args);

class Chat
{
public:
	std::string lines[10];
	glm::vec3 linesColor[10];
	std::string newLine = "";
	bool open = false;
	unsigned int pointer = 0;

	Chat()
	{
		for (int i = 0; i < 10; i++) {
			lines[i] = "";
			linesColor[i] = glm::vec3();
		}
	}

	void Say(std::string text, glm::vec3 color = glm::vec3(), CommandCallback callback = NULL)
	{

		if (text[0] == '/')
		{
			std::istringstream iss(text);
			
			std::vector<std::string> tokens = std::vector<std::string>{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
			if (text.size() <= 1 || tokens.size() < 1)
				return;
			std::string cmd = tokens[0].substr(1, tokens[0].size() - 1);
			tokens.erase(tokens.begin());
			if (callback != 0)
			{
				callback(cmd, tokens);
			}
			return;
		}
		for (int i = 8; i > 0; i--) {
			lines[i] = lines[i - 1];
			linesColor[i] = linesColor[i - 1];
		}
		lines[0] = text;
		linesColor[0] = color;
	}

	void EnterPressed(CommandCallback callback)
	{
		open = !open;
		Settings::Pause = open;
		if (!open && newLine.size() > 0)
		{
			Say(newLine, glm::vec3(), callback);
			newLine = "";
			pointer = 0;
		}
	}

	void Write(char keycode)
	{
		newLine.insert(newLine.begin() + pointer, keycode);
		++pointer;
	}
	void MovePointer(int dir)
	{
		pointer += dir;
		if (pointer < 0)
			pointer = 0;
		if (pointer > newLine.size())
			pointer = newLine.size();
	}

	void Delete()
	{
		if (newLine.size() > 0)
			newLine.erase(newLine.begin() + --pointer);
	}
};

