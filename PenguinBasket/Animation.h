#pragma once

class Animation
{
private:
	int lastFrame = 0;
public:
	char* texturePath = "";
	int frameCount = 1;
	int offsetX = 0;
	int offsetY = 0;

	int frameTime = 0;
	int currentFrame = 0;
	int currentLine = 0;
	int lineCount = 1;
	bool freeze = false;

	Animation()
	{
	}

	void Update()
	{
		if (freeze)
			return;

		++lastFrame;
		if (lastFrame >= frameTime)
		{
			lastFrame = 0;
			NextFrame();
		}
	}

	void NextFrame()
	{
		currentFrame = (currentFrame + 1) % frameCount;
	}
};

