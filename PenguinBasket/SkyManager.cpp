#include "SkyManager.h"


SkyManager::SkyManager(int mapWidth)
{
	this->MapWidth = mapWidth;
	for (int i = 0; i < CloudCount; i++)
	{
		Cloud* cloud = new Cloud();
		cloud->pos = glm::vec2(i * mapWidth / (float) CloudCount, rand() % 100 / 50.0 + 5);
		cloud->size = glm::vec2(4.0f, 1.0f) * (rand() % 101 / 40.0f + 1.0f);
		cloud->type = rand() % 2;
		clouds.push_back(cloud);
	}
	std::sort(clouds.begin(), clouds.end(), Cloud::comp);
	Profiler::Time = &Time;
	
	for (int i = 0; i < rainW; i++)
	{
		for (int j = 0; j < rainH; j++)
		{
			rain[j * rainW + i] = new Rain({ glm::vec2((i + ((float) (rand() % 101) / 100.0f)) * 1280.0f / (float) rainW, (j + ((float) (rand() % 101) / 100.0f)) * 720.0f / (float) rainH), 1 });
		}
	}
}

void SkyManager::Update()
{
	glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
	lightLevel = (int)Time;
	Time += (clock() - lastTimeAdd) / 1000.0; // 1 MIN = 1 HOUR
	lastTimeAdd = clock();
	if (Time > 24.0)
		Time = 0;

	for (std::vector<Cloud*>::iterator it = clouds.begin(); it != clouds.end(); ++it)
	{
		//(*it)->Update();
		(*it)->pos.x += SkySpeed;
		if ((*it)->pos.x > MapWidth)
			(*it)->pos.x = -5;
	}
	if (isRaining) {
		glm::vec2 vel = glm::vec2(1, 2) * 2.0f;
		for (int i = 0; i < rainW * rainH; i++)
		if (rain[i] != nullptr)
			rain[i]->pos += vel;
	}
}