#include "Map.h"

Map* Map::map = nullptr;

Map::Map(const int width, const int height, char* mapData)
{
	Map::map = this;
	unsigned int start = clock();

	this->Width = width;
	this->Height = height;
	Noise* noiser = new Noise((unsigned int)time(NULL));

	heightMap = new int[width];
	for (int i = 0; i < Width; i++) {
		heightMap[i] = (int) (noiser->eval(i / 20.0f) * Height);
	}

	walls = new Wall*[Width];
	for (int i = 0; i < Width; i++) {
		walls[i] = new Wall[Height];
		for (int j = 0; j < Height; j++) {
			walls[i][j].Id = 0;
		}
	}
	blocks = new Tile*[Width];
	for (int i = 0; i < Width; i++) {
		blocks[i] = new Tile[Height];
	}

	if (!Profiler::Multiplayer)
	{

		for (int i = 0; i < Width; i++) {
			int top = heightMap[i];
			for (int j = 0; j < Height; j++) {
				walls[i][j].Id = Block::Dirt->Id;
				if (rand() % 101 > 50 || true || j > top && j < top + 3)
					blocks[i][j].Id = Block::Dirt->Id;
			}
		}
		for (int i = 0; i < Width; i++) {
			int top = heightMap[i];
			for (int j = 0; j <= top; j++) {
				walls[i][j].Id = 0;
				blocks[i][j].Id = 0;
			}
		}
		printf("World tiles fill: %d ms. \n", clock() - start);
		start = clock();

		for (int i = 0; i < 0; i++)
			CelluarAuto(Height, i);

		printf("Celluar Auto: %d ms. \n", clock() - start);
		start = clock();

		/*int k = 0;
		while (k < 100)
		{
			int i = rand() % Width;
			int j = rand() % height;
			if (GetBlock(i, j) == 0)
				continue;
			k += GenerateVein(i, j, Block::Stone->Id, 5);
		}*/

		printf("Veins generation: %d ms. \n", (clock() - start));
	}
	else
	{
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				blocks[i][j].Id = mapData[i + j * width];
			}
		}
	}


	start = clock();
	
	for (int i = 0; i < Width; i++) {
		for (int j = 0; j < Height; j++) {
			if (blocks[i][j].Id != 0)
				blocks[i][j].connectionIndex = GetConnectionIndex(i, j);
		}
	}
	for (int i = 0; i < Width; i++) {
		for (int j = 0; j < Height; j++) {
			if (walls[i][j].Id != 0)
				walls[i][j].connectionIndex = GetConnectionIndex(i, j, true);
		}
	}
	printf("Tiles connections: %d ms. \n", clock() - start);
	start = clock();

	for (int i = 0; i < Width; i++) {
		for (int j = 0; j < Height; j++) {
			if (blocks[i][j].Id == 0 && walls[i][j].Id == 0)
			{
				blocks[i][j].lightSource = MAX_LIGHT;
				if (NearLive(i, j, 1) == 0)
					blocks[i][j].lightValue = MAX_LIGHT;
			}
		}
	}

	minX = 0;
	minY = 0;
	maxX = width;
	maxY = height;
	CalculateLights(true);

	printf("Lights calculations: %d ms. \n", clock() - start);
	start = clock();

	player = new Player();
	Respawn();
	entities.push_back(player);

	skyManager = new SkyManager(this->Width);
}

Map::~Map()
{
}

void Map::CalculateLights(bool firstTime) {
	if (!firstTime)
	{
		for (int i = minX; i < maxX; i++)
		{
			for (int j = minY; j < maxY; j++)
			{
				blocks[i][j].lightValue = 0;
			}
		}
	}

	for (int i = minX; i < maxX; i++)
	{
		for (int j = minY; j < maxY; j++)
		{
			if (blocks[i][j].lightSource > 0) {
				UpdateLight(i, j, blocks[i][j].lightSource);
			}
		}
	}
	Entity* entity;
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		entity = (*it);
		if (entity->GetLight() > 0)
			UpdateLight((int) entity->shape->GetCenterX(), (int) entity->shape->GetCenterY(), entity->GetLight());
	}
}

void Map::Update()
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end();)
	{
		if ((*it)->active)
			++it;
		else {
			delete *it;
			it = entities.erase(it);
		}
	}

	HandleCollision();
	HandleEvents();

	Entity* entity;
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		entity = (*it);
		if (entity->OnScreen == false)
			continue;
		if (entity->GetLight() > 0 && ((int) entity->shape->GetCenterX() != (int) (entity->prevPos.x + entity->GetSize().x / 2.0) || (int) entity->shape->GetCenterY() != (int) (entity->prevPos.y + entity->GetSize().y / 2.0)))
			lightDirty = true;
	}
	skyManager->Update();
}

void Map::UpdateLight(int i, int j, float level, int previ, int prevj, bool recalculate)
{
	if (level <= 1 || !ValidateBlock(i, j))
		return;
	if (blocks[i][j].lightValue < level || (recalculate && blocks[i][j].lightValue == level))
		blocks[i][j].lightValue = level;
	else
		return;
	lightUpdated++;
	if (blocks[i][j].Id > 0)
		--level;
	if (previ != i - 1 || prevj != j)
		UpdateLight(i - 1, j, level - 1, i, j);
	if (previ != i + 1 || prevj != j)
		UpdateLight(i + 1, j, level - 1, i, j);
	if (previ != i || prevj != j - 1)
		UpdateLight(i, j - 1, level - 1, i, j);
	if (previ != i || prevj != j + 1)
		UpdateLight(i, j + 1, level - 1, i, j);
}

int Map::GetBlock(int i, int j)
{
	if (!ValidateBlock(i, j))
		return 0;
	return blocks[i][j].Id;
}

bool Map::IsSolid(int i, int j)
{
	if (!ValidateBlock(i, j))
		return false;
	return Block::GetBlockById(blocks[i][j].Id)->IsSolid();
}

int Map::GetWall(int i, int j)
{
	if (!ValidateBlock(i, j))
		return 0;
	return walls[i][j].Id;
}

void Map::SetBlock(int i, int j, int id)
{
	if (GetBlock(i, j) == id || !ValidateBlock(i, j))
		return;
	if (id == 0)
	{
		blocks[i][j].Id = 0;
		if (walls[i][j].Id == 0)
			blocks[i][j].lightSource = MAX_LIGHT;
	}
	else {
		Block* block = Block::GetBlockById(id);
		blocks[i][j].Id = (char) id;
		blocks[i][j].lightSource = block->GetLightEmit();
		blocks[i][j].hardness = block->GetHardness();
		UpdateBlock(i, j);
	}
	UpdateNearBlocks(i, j);
	lightDirty = true;
}

void Map::UpdateNearBlocks(int i, int j)
{
	UpdateBlock(i - 1, j);
	UpdateBlock(i, j - 1);
	UpdateBlock(i + 1, j);
	UpdateBlock(i, j + 1);
}

void Map::DestroyBlock(int i, int j)
{
	if (GetBlock(i, j) == 0)
		return;
	int id = blocks[i][j].Id;

	SetBlock(i, j, 0);

	EntityItem* item = new EntityItem(Block::GetBlockById(id)->GetDrop());
	item->SetPosition(glm::vec2(i + rand() % 500 / 1000.0, j + rand() % 500 / 1000.0));
	drops.push_back(item);
	entities.push_back(item);
}

bool Map::TryBuildBlock(int i, int j, int id)
{
	if (GetBlock(i, j) != 0 || IntersectsEntities(new Box(i, j, 1, 1)))
		return false;
	SetBlock(i, j, id);
	return true;
}

char Map::GetConnectionIndex(int i, int j, bool wall)
{
	int index = 0;
	if (wall)
	{
		int id = GetWall(i, j);
		if (id == 4)
			id = 3;

		if (GetWall(i, j - 1) < id) {
			index |= 1;
		}
		if (GetWall(i + 1, j) < id) {
			index |= 2;
		}
		if (GetWall(i, j + 1) < id) {
			index |= 4;
		}
		if (GetWall(i - 1, j) < id) {
			index |= 8;
		}

	} else {
		int id = GetBlock(i, j);

		if (GetBlock(i, j - 1) < id) {
			index |= 1;
		}
		if (GetBlock(i + 1, j) < id) {
			index |= 2;
		}
		if (GetBlock(i, j + 1) < id) {
			index |= 4;
		}
		if (GetBlock(i - 1, j) < id) {
			index |= 8;
		}

	}
	return index;
}

void Map::HandleCollision() {
	Entity* entity;
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		entity = (*it);
		entity->onGround = false;
		entity->shape->SetPos(entity->pos.x, entity->pos.y);

		if (!entity->Collidable)
			continue;

		Box* tempBox = new Box(0, 0, 1, 1);
		glm::vec2 maxDepth = glm::vec2();
		glm::vec2 tempDepth;

		for (int i = (int) entity->pos.x - 2; i <= (int) entity->pos.x + 2; i++)
		{
			for (int j = (int) entity->pos.y - 2; j <= (int) entity->pos.y + 2; j++)
			{
				if (GetBlock(i, j) == 0 || !IsSolid(i, j))
					continue;

				tempBox->SetPos(i, j);
				if (!entity->shape->Intersects(tempBox, tempDepth.x, tempDepth.y))
					continue;
				if (abs(tempDepth.x) > abs(tempDepth.y))
				{
					if (entity->pos.y > j) {
						if (GetBlock(i, j + 1) != 0)
							continue;
						entity->pos.y = j + 1;
						if (entity->vel.y < 0)
							entity->vel.y = 0;
					}
					else {
						if (GetBlock(i, j - 1) != 0)
							continue;
						entity->onGround = true;
						entity->pos.y = j - entity->shape->GetH();
						if (entity->vel.y > 0)
							entity->vel.y = 0;
					}
				}
				else {
					if (entity->pos.x < i) {
						if (GetBlock(i - 1, j) != 0)
							continue;
						entity->pos.x = i - entity->shape->GetW();
						if (entity->vel.x > 0)
							entity->vel.x = 0;
					}
					else {
						if (GetBlock(i + 1, j) != 0)
							continue;
						entity->pos.x = i + 1;
						if (entity->vel.x < 0)
							entity->vel.x = 0;
					}
				}
			}
		}
	}
}

void Map::CelluarAuto(int minlevel, int it)
{
	for (int i = 0; i < Width; i++) {
		for (int j = Height - minlevel; j < Height; j++) {
			blocks[i][j].Id = NearLive(i, j, 1) > 4 ? 3 : 0;
		}
	}
}

int Map::NearLive(int i, int j, int size)
{
	int nearLive = 0;
	for (int x = i - size; x <= i + size; x++) {
		for (int y = j - size; y <= j + size; y++){
			if(GetBlock(x, y) > 0)
				nearLive++;
		}
	}
	return nearLive;
}

int Map::GenerateVein(int i, int j, int id, int size)
{
	if (size == 0 || GetBlock(i, j) != 3)
		return 0;
	int l = 1;
	blocks[i][j].Id = id;
	if (rand() % 2)
		l += GenerateVein(i - 1, j, id, size - 1);
	if (rand() % 2)
		l += GenerateVein(i + 1, j, id, size - 1);
	if (rand() % 2)
		l += GenerateVein(i, j - 1, id, size - 1);
	if (rand() % 2)
		l += GenerateVein(i, j + 1, id, size - 1);
	return l;
}

float Map::GetAvgLight(int i, int j)
{
	float topleft = GetLight(i - 1, j - 1);
	float bottomleft = GetLight(i - 1, j);
	float topright = GetLight(i, j - 1);
	float bottomright = GetLight(i, j);
	return (topleft + topright + bottomleft + bottomright) / 4.0f;
};

bool Map::IntersectsEntities(Box* box)
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		float f;
		if ((*it)->shape->Intersects(box, f, f))
			return true;
	}
	return false;
}

void Map::HandleEvents()
{
	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if ((*it)->OnScreen == false || (*it) == player)
			continue;

		float f;
		if ((*it)->shape->Intersects(player->shape, f, f))
			(*it)->OnPlayerCollision();
	}
	
	Box* temp = new Box(0, 0, 1, 1);
	for (std::vector<EntityItem*>::iterator it = drops.begin(); it != drops.end();)
	{
		(*it)->Gravity = true;
		(*it)->Collidable = true;

		if ((*it)->OnScreen == false) {
			++it;
			continue;
		}
		double d = glm::distance((*it)->GetCenter(), player->GetCenter());
		if (d > player->reachDistance || player->inventory->IsFull() && player->inventory->FindSlot((*it)->item->Id) == -1) {
			++it;
			continue;
		}
		temp->SetPos((*it)->pos.x, (*it)->pos.y);
		float f;
		if(player->shape->Intersects(temp, f, f)) {
			if (player->inventory->AddItem((*it)->item.get()))
			{
				(*it)->active = false;
				it = drops.erase(it);
				continue;
			}
		}
		(*it)->Gravity = false;
		(*it)->Collidable = false;
		(*it)->vel += glm::normalize(player->GetCenter() - (*it)->GetCenter()) * 1.5f;
		++it;
	}
}

void Map::Respawn()
{
	player->SetPosition(glm::vec2(Width / 2, GetTop(Width / 2) - 4));
}