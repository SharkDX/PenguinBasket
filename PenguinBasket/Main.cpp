#include "Main.h"
Map* Main::map;
int Main::lightMode = 0;
HUD* Main::hud;
bool Main::MapReady = false;
int Main::lastPing = 0;
Client* Main::m_Client = nullptr;

Main::Main(GLFWwindow* window)
{
}

void Main::Init(GLFWwindow* window) {
	m_Client = new Client();
	m_Client->processPacketFunc = &Main::ProcessPacket;
	//Profiler::Multiplayer = m_Client->Connect("127.0.0.1"/* "77.126.103.88" */, 1234);
	//m_Client->Start();
	
	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1, 1, 1, 1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	Projection = glm::ortho(0.0, (double) screenSize.x, (double) screenSize.y, 0.0, -1.0, 1.0);
	View = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	mWaterFbo = new FrameBufferObject(screenSize.x, screenSize.y);

	glfwSetScrollCallback(window, &Main::ScrollWheelCallback);
	glfwSetKeyCallback(window, &Main::KeyCallback);
	glfwSetCharCallback(window, &Main::CharCallback);
	glfwSetMouseButtonCallback(window, &Main::MouseButtonCallback);


	hud = new HUD(window);
	drawer = new Drawer();
	printer = new Printer();

	Item::InitItems();

	m_Client->SendPacket(std::shared_ptr<Packet>(new PacketHandshake(PROTOCOL_VERSION, 0)));
	if (!Profiler::Multiplayer) {
		map = new Map(100, 100);
		MapReady = true;
	}

	map->player->inventory->AddItem(Item::pickaxe);
	//map->player->inventory->AddItem(Item::ingot);
}

void Main::LoadResources() {

	unsigned int start = clock();

	ResourceManager::CreateInstance();
	ResourceManager* resources = ResourceManager::GetInstance();

	resources->LoadShader("textured.vert", "", "textured.frag");
	resources->LoadShader("colored.vert", "", "colored.frag");
	resources->LoadShader("blur.vert", "", "blur.frag");
	resources->LoadShader("font.vert", "", "font.frag");
	resources->LoadShader("framed.vert", "", "framed.frag");

	resources->LoadTextureTransparent("cursor.png");
	resources->LoadTextureTransparent("circle.png");
	resources->LoadTextureTransparent("clouds.png");
	resources->LoadTextureTransparent("blocks.png");
	resources->LoadTextureTransparent("animationTest.png");
	resources->LoadTextureTransparent("Items.png");
	resources->LoadTextureTransparent("sheep.png");
	resources->LoadTextureTransparent("rain.png");
	resources->LoadTextureTransparent("hand.png");

	resources->LoadTextureTransparent("slot.png");
	resources->LoadShader("hud.vert", "", "hud.frag");

	printf("Content loading: %d ms. \n", clock() - start);
	start = clock();
	}


Main::~Main()
{
}

void Main::Update(GLFWwindow* window, float deltaTime)
{
	if (!MapReady || Profiler::Pause)
		return;	
	map->player->isWalking = false;
	float speed = 8.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed *= 4.0;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		map->player->SetVelocityX(speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		map->player->SetVelocityX(-speed);
	if (map->player->onGround && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		map->player->SetVelocityY(-6.0f);
	}
	if (Profiler::Multiplayer)
	{
		m_Client->SendPacket(std::make_shared<PacketPlayerPosition>(m_Client->clientId, map->player->pos.x, map->player->pos.y, map->player->vel.x, map->player->vel.y));
		if (clock() - lastPing > 1000) {
			m_Client->SendPacket(std::shared_ptr<Packet>(new PacketPing()));
			lastPing = clock();
		}
	}
	if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS)
		SOIL_save_screenshot
		(
		std::string("screenshot").append(std::to_string(time(NULL))).append(".bmp").c_str(),
		SOIL_SAVE_TYPE_BMP,
		0, 0, (int) screenSize.x, (int) screenSize.y
		);
	for (std::vector<Entity*>::iterator it = map->entities.begin(); it != map->entities.end(); ++it)
	{
		(*it)->Update(window, deltaTime);
	}
	for (std::vector<Water*>::iterator it = waters.begin(); it != waters.end(); ++it)
	{
		(*it)->Update(window, deltaTime, waters);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		xpos += CameraPos.x;
		ypos += CameraPos.y;

		std::shared_ptr<Item> item = map->player->inventory->GetItem(map->player->selectedSlot);
		if (item != nullptr)
		{
			map->player->inventory->UseItem(map->player->selectedSlot, xpos / BLOCK_SIZE, ypos / BLOCK_SIZE);
			/*
			if (Profiler::Multiplayer)
			m_Client->SendPacket(std::make_shared<PacketCreateBlock>(item->Id, 0, (int)(xpos / BLOCK_SIZE), (int)(ypos / BLOCK_SIZE))); //TODO Send chunk position instead of constant 0
			TODO Put m_Clients in Map.h and handle it in ItemBlock.cpp
			*/
		}
	}
	map->Update();
}

void Main::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!MapReady)
		return;

	CalculateCameraProperties();

	if (map->lightDirty) {
		map->CalculateLights();
		map->lightDirty = false;
	}

	glm::vec2 arm1 = map->player->hand1;
	glm::vec2 arm2 = map->player->hand2;

	arm1 += map->player->pos + glm::vec2(0.5f, 0.5f);
	arm2 += map->player->pos + glm::vec2(0.5f, 0.5f);

	Profiler::DRAW_CALLS = 0;

	ResourceManager* resources = ResourceManager::GetInstance();
	Shader* texturedShader = resources->GetShader("textured");
	GLuint textureLoc = texturedShader->GetUniformLocation("blockTexture");
	GLuint posLoc = texturedShader->GetUniformLocation("blockPos");
	GLuint blockIndexLoc = texturedShader->GetUniformLocation("blockIndex");
	GLuint spriteIndexLoc = texturedShader->GetUniformLocation("spriteIndex");
	GLuint mvpLoc = texturedShader->GetUniformLocation("MVP");
	GLuint lightLoc = texturedShader->GetUniformLocation("lightMap");
	GLuint blockRenderLoc = texturedShader->GetUniformLocation("blockRender");
	GLuint sizetexLoc = texturedShader->GetUniformLocation("size");

	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, drawer->mVbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*) 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*) 8);
	
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

	RenderClouds(framedShader);

	if (map->skyManager->isRaining)
		RenderRain(framedShader);

	framedShader->Release();
	
	texturedShader->Bind();
	posLoc = texturedShader->GetUniformLocation("blockPos");
	mvpLoc = texturedShader->GetUniformLocation("MVP");
	glUniformMatrix4fv(mvpLoc, 1, false, &(Projection * View)[0][0]);
	glUniform1f(textureLoc, 0);
	glUniformMatrix4fv(mvpLoc, 1, false, &(Projection * View)[0][0]);
	glUniform1f(blockRenderLoc, 1.0f);
	glBindTexture(GL_TEXTURE_2D, resources->GetTexture("blocks"));
	for (int i = map->minX; i < map->maxX; i++)
	{
		for (int j = map->minY; j < map->maxY; j++)
		{
			if (i < (int) (CameraPos.x / BLOCK_SIZE) || j < (int) (CameraPos.y / BLOCK_SIZE) ||
				i >(int) ((CameraPos.x + 1280) / BLOCK_SIZE) || j >(int) ((CameraPos.y + 720) / BLOCK_SIZE) ||
				map->GetWall(i, j) == 0 || map->GetBlock(i, j) != 0)
				continue;
			int id = map->GetWall(i, j);
			glUniform1f(blockIndexLoc, Block::GetBlockById(id)->GetTextureCoord());
			glUniform2f(posLoc, i * BLOCK_SIZE, j * BLOCK_SIZE);
			glUniform1f(spriteIndexLoc, 4.0f);
			if (lightMode == 0)
				glUniform4f(lightLoc, map->GetAvgLight(i, j) / 2.0f, map->GetAvgLight(i + 1, j) / 2.0f, map->GetAvgLight(i, j + 1) / 2.0f, map->GetAvgLight(i + 1, j + 1) / 2.0f);
			else if (lightMode == 1)
				glUniform4f(lightLoc, map->GetLight(i, j) / 2.0f, map->GetLight(i, j) / 2.0f, map->GetLight(i, j) / 2.0f, map->GetLight(i, j) / 2.0f);
			drawer->DrawBlock();
		}
	}
	for (int i = map->minX; i < map->maxX; i++)
	{
		for (int j = map->minY; j < map->maxY; j++)
		{
			if (i < (int) (CameraPos.x / BLOCK_SIZE) || j < (int) (CameraPos.y / BLOCK_SIZE) ||
				i >(int) ((CameraPos.x + 1280) / BLOCK_SIZE) || j >(int) ((CameraPos.y + 720) / BLOCK_SIZE) ||
				map->GetWall(i, j) == 0)
				continue;
			int id = map->GetWall(i, j);
			glUniform1f(blockIndexLoc, Block::GetBlockById(id)->GetTextureCoord());
			glUniform4f(lightLoc, map->GetLight(i, j) / 2.0f, map->GetLight(i, j) / 2.0f, map->GetLight(i, j) / 2.0f, map->GetLight(i, j) / 2.0f);
			if (map->walls[i][j].connectionIndex & 1) {
				glUniform2f(posLoc, i * BLOCK_SIZE, (j - 1) * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 1.0f);
				drawer->DrawBlock();
			}
			if (map->walls[i][j].connectionIndex & 2) {
				glUniform2f(posLoc, (i + 1) * BLOCK_SIZE, j * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 5.0f);
				drawer->DrawBlock();
			}
			if (map->walls[i][j].connectionIndex & 4) {
				glUniform2f(posLoc, i * BLOCK_SIZE, (j + 1) * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 7.0f);
				drawer->DrawBlock();
			}
			if (map->walls[i][j].connectionIndex & 8) {
				glUniform2f(posLoc, (i - 1) * BLOCK_SIZE, j * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 3.0f);
				drawer->DrawBlock();
			}

		}
	}
	texturedShader->Release();
	
	/*Shader coloredShader = resources->GetShader("colored");
	coloredShader->Bind();
	mvpLoc = coloredShader->GetUniformLocation("MVP");
	posLoc = coloredShader->GetUniformLocation("pos");
	GLuint sizeLoc = coloredShader->GetUniformLocation("size");
	

	GLuint colorLoc = coloredShader->GetUniformLocation("inColor");
	glUniformMatrix4fv(mvpLoc, 1, false, &(Projection * View)[0][0]);
	for (std::vector<Entity*>::iterator it = map->entities.begin(); it != map->entities.end(); ++it)
	{
		if ((*it)->pos.x < map->minX || (*it)->pos.y < map->minY || (*it)->pos.x > map->maxX || (*it)->pos.y > map->maxY)
		{
			(*it)->OnScreen = false;
			continue;
		}
		(*it)->OnScreen = true;
		if ((*it)->animation != NULL || (map->drops.size() > 0 && typeid(*(*it)) == typeid(EntityItem)))
			continue;
		float light = map->GetLight((int)(*it)->shape->GetCenterX(), (int)(*it)->shape->GetCenterY());
		glUniform4f(colorLoc, light, light, light, 1);
		glUniform2f(posLoc, (*it)->GetPosition().x * BLOCK_SIZE, (*it)->GetPosition().y * BLOCK_SIZE);
		glUniform2f(sizeLoc, (*it)->GetSize().x, (*it)->GetSize().y);
		drawer->DrawBlock();
	}
	coloredShader->Release();*/
	
	framedShader->Bind();

	glUniformMatrix4fv(framed_mvpLoc, 1, false, &(Projection * View)[0][0]);
	glUniform1f(framed_alphaLoc, 1);

	if (map->player->hand2.x != -1)
	{
		glBindTexture(GL_TEXTURE_2D, resources->GetTexture("hand"));
		glUniform1f(framed_lightLoc, map->GetLight((int) map->player->shape->GetCenterX(), (int) map->player->shape->GetCenterY()));
		glUniform2f(framed_posLoc, (int) (arm2.x * BLOCK_SIZE) - 1, (int) (arm2.y * BLOCK_SIZE) - 2);
		glUniform2f(framed_sizeLoc, 0.25f, 0.25f);
		glUniform1f(framed_frameLoc, 0);
		glUniform1f(framed_frameLoc, 0);
		glUniform1f(framed_lineCountLoc, 1);
		glUniform1f(framed_totalFramesLoc, 1);
		drawer->DrawBlock();
	}

	for (std::vector<Entity*>::iterator it = map->entities.begin(); it != map->entities.end(); ++it)
	{
		if ((*it)->pos.x < map->minX || (*it)->pos.y < map->minY || (*it)->pos.x > map->maxX || (*it)->pos.y > map->maxY)
		{
			(*it)->OnScreen = false;
			continue;
		}
		(*it)->OnScreen = true;
		if ((*it)->animation == NULL)
			continue;
		float light = map->GetLight((int) (*it)->shape->GetCenterX(), (int) (*it)->shape->GetCenterY());
		glUniform1f(framed_lightLoc, light);
		glBindTexture(GL_TEXTURE_2D, resources->GetTexture((*it)->animation->texturePath));
		glUniform2f(framed_posLoc, (int)((*it)->GetPosition().x * BLOCK_SIZE + (*it)->animation->offsetX), (int)((*it)->GetPosition().y * BLOCK_SIZE + (*it)->animation->offsetY));
		glUniform2f(framed_sizeLoc, 2.0f, 2);
		glUniform1f(framed_frameLoc, (*it)->animation->currentFrame + (*it)->animation->currentLine * (*it)->animation->frameCount);
		glUniform1f(framed_lineCountLoc, (*it)->animation->frameCount);
		glUniform1f(framed_totalFramesLoc, (*it)->animation->lineCount * (*it)->animation->frameCount);
		glUniform1f(framed_dirLoc, (*it)->direction);
		drawer->DrawBlock();
	}

	glBindTexture(GL_TEXTURE_2D, resources->GetTexture("blocks"));
	glUniform1f(framed_totalFramesLoc, 16);
	glUniform1f(framed_lineCountLoc, 16);
	glUniform1f(framed_dirLoc, 1);

	for (std::vector<EntityItem*>::iterator it = map->drops.begin(); it != map->drops.end(); ++it)
	{
		if (!(*it)->item->block)
			continue;
		float light = map->GetLight((int) (*it)->shape->GetCenterX(), (int) (*it)->shape->GetCenterY());

		glUniform1f(framed_lightLoc, light);
		glUniform2f(framed_posLoc, (int) ((*it)->GetPosition().x * BLOCK_SIZE) - 16, (int) ((*it)->GetPosition().y * BLOCK_SIZE) - 16);
		glUniform2f(framed_sizeLoc, 1.4, 1.4);
		glUniform1f(framed_frameLoc, Block::GetBlockById((std::dynamic_pointer_cast<ItemBlock>((*it)->item))->blockId)->GetTextureCoord());
		drawer->DrawBlock();
	}

	glBindTexture(GL_TEXTURE_2D, resources->GetTexture("Items"));
	for (std::vector<EntityItem*>::iterator it = map->drops.begin(); it != map->drops.end(); ++it)
	{
		if ((*it)->item->block)
			continue;
		float light = map->GetLight((int) (*it)->shape->GetCenterX(), (int) (*it)->shape->GetCenterY());

		glUniform1f(framed_lightLoc, light);
		glUniform2f(framed_posLoc, (int) ((*it)->GetPosition().x * BLOCK_SIZE) - 16, (int) ((*it)->GetPosition().y * BLOCK_SIZE) - 16);
		glUniform2f(framed_sizeLoc, 1.4, 1.4);
		glUniform1f(framed_frameLoc, (*it)->item->textureIndex);
		drawer->DrawBlock();
	}

	std::shared_ptr<Item> item = map->player->inventory->GetItem(map->player->selectedSlot);
	if (map->player->swinging && item != nullptr && item->block == false)
	{
		glUniform1f(framed_totalFramesLoc, 16);
		glUniform1f(framed_lineCountLoc, 16);
		glUniform1f(framed_dirLoc, 1);

		glm::vec3 o = glm::vec3(0, 32, 0);
		glm::vec3 p = glm::vec3((int) ((map->player->GetPosition().x) * BLOCK_SIZE + (map->player->direction == 1.0f ? 16 : 8)), (int) (map->player->GetPosition().y * BLOCK_SIZE), 0);
		glUniformMatrix4fv(framed_mvpLoc, 1, false, &(Projection * View * glm::translate(p) * glm::translate(o) * glm::rotate(map->player->itemRot, glm::vec3(0, 0, 1)) * glm::translate(-o))[0][0]);
		glUniform1f(framed_lightLoc, 1);
		//glUniform2f(framed_posLoc, (int) ((map->player->GetPosition().x) * BLOCK_SIZE + (int) (map->player->GetSize().x * BLOCK_SIZE)), (int) ((map->player->GetPosition().y) * BLOCK_SIZE));
		glUniform2f(framed_posLoc, 0.0, 0.0);
		glUniform2f(framed_sizeLoc, 1.0, 1.0);
		glUniform1f(framed_frameLoc, item->textureIndex);
		drawer->DrawBlock();
	}
	
	if (map->player->hand1.x != -1)
	{
		glUniformMatrix4fv(framed_mvpLoc, 1, false, &(Projection * View)[0][0]);
		glUniform1f(framed_alphaLoc, 1);
		glBindTexture(GL_TEXTURE_2D, resources->GetTexture("hand"));
		glUniform1f(framed_lightLoc, map->GetLight((int) map->player->shape->GetCenterX(), (int) map->player->shape->GetCenterY()));
		glUniform2f(framed_posLoc, (int) (arm1.x * BLOCK_SIZE) - 4, (int) (arm1.y * BLOCK_SIZE) - 4);
		glUniform2f(framed_sizeLoc, 0.25f, 0.25f);
		glUniform1f(framed_frameLoc, 0);
		glUniform1f(framed_frameLoc, 0);
		glUniform1f(framed_lineCountLoc, 1);
		glUniform1f(framed_totalFramesLoc, 1);
		drawer->DrawBlock();
	}
	framedShader->Release();

	texturedShader->Bind();
	posLoc = texturedShader->GetUniformLocation("blockPos");
	mvpLoc = texturedShader->GetUniformLocation("MVP");
	glUniformMatrix4fv(mvpLoc, 1, false, &(Projection * View)[0][0]);
	glUniform1f(textureLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, resources->GetTexture("blocks"));
	for (int i = map->minX; i < map->maxX; i++)
	{
		for (int j = map->minY; j < map->maxY; j++)
		{
			if (i < (int) (CameraPos.x / BLOCK_SIZE) || j < (int) (CameraPos.y / BLOCK_SIZE) ||
				i >(int) ((CameraPos.x + 1280) / BLOCK_SIZE) || j >(int) ((CameraPos.y + 720) / BLOCK_SIZE) ||
				map->GetBlock(i, j) == 0)
				continue;
			int id = map->GetBlock(i, j);
			glUniform1f(blockIndexLoc, Block::GetBlockById(id)->GetTextureCoord());
			glUniform2f(posLoc, i * BLOCK_SIZE, j * BLOCK_SIZE);
			glUniform1f(spriteIndexLoc, 4.0f);
			if (lightMode == 0)
				glUniform4f(lightLoc, map->GetAvgLight(i, j), map->GetAvgLight(i + 1, j), map->GetAvgLight(i, j + 1), map->GetAvgLight(i + 1, j + 1));
			else if (lightMode == 1)
				glUniform4f(lightLoc, map->GetLight(i, j), map->GetLight(i, j), map->GetLight(i, j), map->GetLight(i, j));
			drawer->DrawBlock();
		}
	}
	for (int i = map->minX; i < map->maxX; i++)
	{
		for (int j = map->minY; j < map->maxY; j++)
		{
			if (i < (int) (CameraPos.x / BLOCK_SIZE) || j < (int) (CameraPos.y / BLOCK_SIZE) ||
				i >(int) ((CameraPos.x + 1280) / BLOCK_SIZE) || j > (int)((CameraPos.y + 720) / BLOCK_SIZE) ||
				map->GetBlock(i, j) == 0)
				continue;
			int id = map->GetBlock(i, j);
			glUniform1f(blockIndexLoc, Block::GetBlockById(id)->GetTextureCoord());

			if (lightMode == 0)
				glUniform4f(lightLoc, map->GetAvgLight(i, j), map->GetAvgLight(i + 1, j), map->GetAvgLight(i, j + 1), map->GetAvgLight(i + 1, j + 1));
			else if (lightMode == 1)
				glUniform4f(lightLoc, map->GetLight(i, j), map->GetLight(i, j), map->GetLight(i, j), map->GetLight(i, j));

			if (map->blocks[i][j].connectionIndex & 1) {
				glUniform4f(lightLoc, map->GetAvgLight(i, j - 1), map->GetAvgLight(i + 1, j - 1), map->GetAvgLight(i, j), map->GetAvgLight(i + 1, j));
				glUniform2f(posLoc, i * BLOCK_SIZE, (j - 1) * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 1.0f);
				drawer->DrawBlock();
			}
			if (map->blocks[i][j].connectionIndex & 2) {
				glUniform4f(lightLoc, map->GetAvgLight(i + 1, j), map->GetAvgLight(i + 2, j), map->GetAvgLight(i + 1, j + 1), map->GetAvgLight(i + 2, j + 1));
				glUniform2f(posLoc, (i + 1) * BLOCK_SIZE, j * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 5.0f);
				drawer->DrawBlock();
			}
			if (map->blocks[i][j].connectionIndex & 4) {
				glUniform4f(lightLoc, map->GetAvgLight(i, j + 1), map->GetAvgLight(i + 1, j + 1), map->GetAvgLight(i, j + 2), map->GetAvgLight(i + 1, j + 2));
				glUniform2f(posLoc, i * BLOCK_SIZE, (j + 1) * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 7.0f);
				drawer->DrawBlock();
			}
			if (map->blocks[i][j].connectionIndex & 8) {
				glUniform4f(lightLoc, map->GetAvgLight(i - 1, j), map->GetAvgLight(i, j), map->GetAvgLight(i - 1, j + 1), map->GetAvgLight(i, j + 1));
				glUniform2f(posLoc, (i - 1) * BLOCK_SIZE, j * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 3.0f);
				drawer->DrawBlock();
			}

			/*if (map->blocks[i][j].connectionIndex & 16) {
				glUniform2f(posLoc, i * BLOCK_SIZE, (j - 1) * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 2.0f);
				drawer->DrawBlock();
			}
			if (map->blocks[i][j].connectionIndex & 32) {
				glUniform2f(posLoc, (i + 1) * BLOCK_SIZE, j * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 8.0f);
				drawer->DrawBlock();
			}
			if (map->blocks[i][j].connectionIndex & 64) {
				glUniform2f(posLoc, i * BLOCK_SIZE, (j + 1) * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 6.0f);
				drawer->DrawBlock();
			}
			if (map->blocks[i][j].connectionIndex & 128) {
				glUniform2f(posLoc, (i - 1) * BLOCK_SIZE, j * BLOCK_SIZE);
				glUniform1f(spriteIndexLoc, 0.0f);
				drawer->DrawBlock();
			}*/
		}
	}
	texturedShader->Release();

	hud->Render(Projection, map->player->inventory, map->player->selectedSlot, map->player->inventoryOpen, printer);

	/*glUniform1f(texturedShader->GetUniformLocation("isBlock"), 0);
	glBindTexture(GL_TEXTURE_2D, resources->GetTexture("circle"));
	mWaterFbo->Bind();
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (std::vector<Water*>::iterator it = waters.begin(); it != waters.end(); ++it)
	{
		glUniform2f(posLoc, (*it)->GetPosition().x * BLOCK_SIZE, (*it)->GetPosition().y * BLOCK_SIZE);
		glUniform1f(spriteIndexLoc, 0);
		//(*it)->Render();
		drawer->DrawBlock(0, 0, 0);
	}
	glClearColor(0.4f, 0.6f, 1.0f, 1.0f);
	mWaterFbo->Unbind();
	texturedShader->Release();*/

	/*Shader* fontShader = resources->GetShader("font");
	fontShader->Bind();

	glUniformMatrix4fv(fontShader->GetUniformLocation("MVP"), 1, false, &Projection[0][0]);
	glUniform1f(fontShader->GetUniformLocation("tex"), 0);
	glActiveTexture(GL_TEXTURE0);

	printer->render_text(std::string("FPS: ").append(std::to_string(Profiler::FPS)).c_str(), 0, 100);
	
	fontShader->Release();*/

	/*Shader* blurShader = resources->GetShader("blur");
	blurShader->Bind();

	mvpLoc = blurShader->GetUniformLocation("Projection");
	textureLoc = blurShader->GetUniformLocation("textureMap");

	glUniform1f(textureLoc, 0);
	glUniformMatrix4fv(mvpLoc, 1, false, &(Projection)[0][0]);
	glBindTexture(GL_TEXTURE_2D, mWaterFbo->textureID);

	drawer->DrawScreen();

	blurShader->Release();*/
}

void Main::CalculateCameraProperties()
{
	CameraPos = map->player->GetPosition() * (float) BLOCK_SIZE - screenSize / 2.0f;
	CameraPos = glm::vec2((int) CameraPos.x, (int) CameraPos.y);
	CameraPos = glm::clamp(CameraPos, glm::vec2(), glm::vec2(map->Width, map->Height) * (float) BLOCK_SIZE - screenSize);

	map->minX = CameraPos.x / (float) BLOCK_SIZE - MAX_LIGHT;
	map->minY = CameraPos.y / (float) BLOCK_SIZE - MAX_LIGHT;
	map->maxX = (CameraPos.x + screenSize.x) / (float) BLOCK_SIZE + MAX_LIGHT;
	map->maxY = (CameraPos.y + screenSize.y) / (float) BLOCK_SIZE + MAX_LIGHT;
	if (map->minX < 0)
		map->minX = 0;
	if (map->minY < 0)
		map->minY = 0;
	if (map->maxX > map->Width)
		map->maxX = map->Width;
	if (map->maxY > map->Height)
		map->maxY = map->Height;

	View = glm::lookAt(glm::vec3(CameraPos.x, CameraPos.y, 1), glm::vec3(CameraPos.x, CameraPos.y, 0), glm::vec3(0, 1, 0));
}

void Main::RenderClouds(Shader* shader)
{
	ResourceManager* resources = ResourceManager::GetInstance();

	GLuint framed_posLoc = shader->GetUniformLocation("pos");
	GLuint framed_mvpLoc = shader->GetUniformLocation("MVP");
	GLuint framed_sizeLoc = shader->GetUniformLocation("size");
	GLuint framed_frameLoc = shader->GetUniformLocation("frame");
	GLuint framed_lineCountLoc = shader->GetUniformLocation("lineCount");
	GLuint framed_dirLoc = shader->GetUniformLocation("dir");
	GLuint framed_textureLoc = shader->GetUniformLocation("frameTexture");
	GLuint framed_totalFramesLoc = shader->GetUniformLocation("totalFrames");
	GLuint framed_alphaLoc = shader->GetUniformLocation("alphaValue");
	GLuint framed_lightLoc = shader->GetUniformLocation("light");

	glBindTexture(GL_TEXTURE_2D, resources->GetTexture("clouds"));
	glUniformMatrix4fv(framed_mvpLoc, 1, false, &(Projection * glm::lookAt(glm::vec3(CameraPos.x, CameraPos.y, 1) * 0.1f, glm::vec3(CameraPos.x, CameraPos.y, 0) * 0.1f, glm::vec3(0, 1, 0)))[0][0]);
	glUniform1f(framed_lineCountLoc, 1);
	glUniform1f(framed_dirLoc, 1);
	glUniform1f(framed_textureLoc, 0);
	glUniform1f(framed_totalFramesLoc, 2);
	glUniform1f(framed_lightLoc, 1.0f);
	for (std::vector<Cloud*>::iterator it = map->skyManager->clouds.begin(); it != map->skyManager->clouds.end(); ++it)
	{
		glUniform1f(framed_alphaLoc, 1.0f - glm::length((*it)->size) / 15.0f + 0.1f);
		glUniform1f(framed_frameLoc, (*it)->type);
		glUniform2f(framed_posLoc, (*it)->pos.x * BLOCK_SIZE, (*it)->pos.y * BLOCK_SIZE);
		glUniform2f(framed_sizeLoc, (*it)->size.x, (*it)->size.y);
		drawer->DrawBlock();
	}
}

void Main::RenderRain(Shader* shader)
{
	ResourceManager* resources = ResourceManager::GetInstance();

	GLuint framed_posLoc = shader->GetUniformLocation("pos");
	GLuint framed_mvpLoc = shader->GetUniformLocation("MVP");
	GLuint framed_sizeLoc = shader->GetUniformLocation("size");
	GLuint framed_frameLoc = shader->GetUniformLocation("frame");
	GLuint framed_lineCountLoc = shader->GetUniformLocation("lineCount");
	GLuint framed_dirLoc = shader->GetUniformLocation("dir");
	GLuint framed_textureLoc = shader->GetUniformLocation("frameTexture");
	GLuint framed_totalFramesLoc = shader->GetUniformLocation("totalFrames");
	GLuint framed_alphaLoc = shader->GetUniformLocation("alphaValue");
	GLuint framed_lightLoc = shader->GetUniformLocation("light");

	glBindTexture(GL_TEXTURE_2D, resources->GetTexture("rain"));
	glUniformMatrix4fv(framed_mvpLoc, 1, false, &(Projection * View)[0][0]);
	glUniform1f(framed_totalFramesLoc, 1);
	glUniform1f(framed_dirLoc, 1.0f);
	glUniform1f(framed_alphaLoc, 1.0f);
	for (int i = 0; i < map->skyManager->rainW * map->skyManager->rainH; i++)
	{
		if (map->skyManager->rain[i] == nullptr)
			continue;
		glUniform1f(framed_frameLoc, 0);
		glUniform2f(framed_posLoc, ceil((CameraPos.x - map->skyManager->rain[i]->pos.x) / 1280.0f) * 1280.0f + map->skyManager->rain[i]->pos.x, ceil((CameraPos.y - map->skyManager->rain[i]->pos.y) / 720.0f) * 720.0f + map->skyManager->rain[i]->pos.y);
		glUniform2f(framed_sizeLoc, 8.0f / 32.0f, 16.0f / 32.0f);
		drawer->DrawBlock();
	}
}

void Main::ScrollWheelCallback(GLFWwindow* window, double x, double y)
{
	map->player->selectedSlot += (int) y;
	map->player->selectedSlot = (map->player->selectedSlot + 10) % 10;
}

void Main::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		hud->chat->EnterPressed(&Main::CommandCallback);
	}
	if (hud->chat->open && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		if (key == GLFW_KEY_BACKSPACE)
		{
			hud->chat->Delete();
		}
		if (key == GLFW_KEY_RIGHT)
			hud->chat->MovePointer(1);
		if (key == GLFW_KEY_LEFT)
			hud->chat->MovePointer(-1);
	}
	else
	{
		if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
			lightMode = (lightMode + 1) % 2;
		if (key == GLFW_KEY_I && action == GLFW_PRESS)
			map->player->inventoryOpen = !map->player->inventoryOpen;
	}
	if (hud->inventoryOpen == false && action == GLFW_PRESS && (key >= GLFW_KEY_0 && key <= GLFW_KEY_9))
	{
		map->player->selectedSlot = key == GLFW_KEY_0 ? 9 : key - GLFW_KEY_1;
	}

	if (Profiler::Pause == false && key == GLFW_KEY_SPACE && action == GLFW_PRESS && map->player->jumpCount > 0)
	{
		map->player->SetVelocityY(-6.0f);
		map->player->jumpCount--;
	}
}

void Main::CommandCallback(std::string command, std::vector<std::string> args)
{
	if (command == "respawn")
	{
		map->Respawn();
	}
	else if (command == "item")
	{
		if (args.size() < 1 || args.size() > 2)
			return;
		int id = atoi(args[0].c_str());
		int count = 1;
		if (args.size() == 2)
			count = atoi(args[1].c_str());
		map->player->inventory->AddItem(Item::items[id], count);
	}
	else
		hud->chat->Say(std::string("'").append(command).append("' is not a valid command."), glm::vec3(0.9, 0, 0));
}

void Main::CharCallback(GLFWwindow* window, unsigned int keycode)
{
	if (hud->chat->open)
	{
		char c = (char) keycode;
		hud->chat->Write(c);
	}
}

void Main::MouseButtonCallback(GLFWwindow* window, int button, int action, int bits)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::shared_ptr<Item> item = map->player->inventory->GetItem(map->player->selectedSlot);
		if (item != nullptr && item->block == false)
		{
			map->player->swinging = true;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		map->player->swinging = false;
		map->player->itemRot = -45;
	}
}

void Main::ProcessPacket(ENetPacket* packet, ENetPeer* peer)
{
	unsigned char header = packet->data[0];
	Client* client = (Client*) peer->data;
	char* c = (char*) packet->data + 1;
	DataBuffer data = DataBuffer(c, packet->dataLength - 1);

	switch (header)
	{
	case PACKET_HAND_SHAKE:
	{
		data.ReadInt();
		short clientId = data.ReadShort();
		m_Client->clientId = clientId;
	}
		break;

	case PACKET_PLAYER_CONNECTED:
	{
		map->mpPlayersMutex.lock();

		PlayerMP* player = new PlayerMP(data.ReadShort(), 0, 0);
		player->Name = data.ReadString();

		map->mpPlayers.insert(std::pair<int, PlayerMP*>(player->Id, player));
		map->entities.push_back(player);

		map->mpPlayersMutex.unlock();
	}
		break;

	case PACKET_PLAYER_DISCONNECTED:
	{
		map->mpPlayersMutex.lock();

		short id = data.ReadShort();
		hud->chat->Say(map->mpPlayers[id]->Name.append(" has left."));

		PlayerMP* player = map->mpPlayers[id];
		map->mpPlayers.erase(id);
		player->active = false;

		map->mpPlayersMutex.unlock();
	}
		break;
	case PACKET_MAP_DATA:
	{
		PacketMapData* mapData = new PacketMapData(data);
		map = new Map(mapData->m_mapWidth, mapData->m_mapHeight, mapData->m_mapData);
		map->player->SetPosition(glm::vec2(mapData->m_spawnX, mapData->m_spawnY));
		MapReady = true;
		m_Client->SendPacket(std::shared_ptr<Packet>(new PacketPing()));
		lastPing = clock();
	}
		break;
	case PACKET_SPAWN_PLAYER:
	{
		hud->chat->Say(std::string("Spawn player packet received ID: ") + std::to_string(data.ReadShort()));
	}
		break;
	case PACKET_PLAYER_POSITION:
	{
		if (!MapReady)
			break;
		short playerID = 0;
		memcpy(&playerID, &packet->data[1], sizeof(short));

		if (map->mpPlayers[playerID] == NULL)
			break;
		float posX = 0;
		memcpy(&posX, &packet->data[3], sizeof(float));

		float posY = 0;
		memcpy(&posY, &packet->data[7], sizeof(float));

		float velX = 0;
		memcpy(&velX, &packet->data[11], sizeof(float));

		float velY = 0;
		memcpy(&velY, &packet->data[15], sizeof(float));

		map->mpPlayers[playerID]->SetPosition(glm::vec2(posX, posY));
		map->mpPlayers[playerID]->SetVelocity(glm::vec2(velX, velY));
	}
		break;
	case PACKET_PING:
	{
		Profiler::Ping = clock() - lastPing;
	}
		break;
	case PACKET_CREATE_BLOCK:
	{
		char blockID = data.ReadChar();
		short chunkPos = data.ReadShort(); //TODO Use that when the engine will run on chunks
		short posX = data.ReadShort();
		short posY = data.ReadShort();
		map->SetBlock(posX, posY, blockID);
	}
		break;
	case PACKET_DESTROY_BLOCK:
	{
		short chunkPos = data.ReadShort(); //TODO Use that when the engine will run on chunks
		short posX = data.ReadShort();
		short posY = data.ReadShort();
		map->DestroyBlock(posX, posY);
	}
		break;
	}
}