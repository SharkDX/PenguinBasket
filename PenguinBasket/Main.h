#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <vector>
#include <SOIL.h>
#include "FrameBufferObject.h"
#include "ResourceManager.h"
#include "Drawer.h"
#include "Map.h"
#include "Water.h"
#include "HUD.h"
#include "Client.h"

class Main
{
public:
	Main(GLFWwindow* window);
	~Main();
	const glm::vec2 screenSize = glm::vec2(1280, 720);

	static HUD* hud;
	Drawer* drawer;
	Printer* printer;
	std::vector<Water*> waters;

	void Init(GLFWwindow* window);
	void LoadResources();
	void Update(GLFWwindow* window, float deltaTime);
	void Render();
	void CalculateCameraProperties();
	void RenderClouds(Shader* shader);
	void RenderRain(Shader* shader);

	static void ScrollWheelCallback(GLFWwindow* window, double x, double y);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int bits);
	static void CharCallback(GLFWwindow* window, unsigned int keycode);
	static void CommandCallback(std::string command, std::vector<std::string> args);
	static void ProcessPacket(ENetPacket* packet, ENetPeer* peer);

	static Map* map;
	static bool MapReady;
	static Client* m_Client;
private:
	static int lightMode;
	glm::mat4x4 View;
	glm::mat4x4 Projection;
	glm::vec2 CameraPos;
	double lastShot = 0;
	FrameBufferObject* mWaterFbo;
	static int lastPing;
};

