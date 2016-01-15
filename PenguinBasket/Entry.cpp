#include <iostream>
#include <string>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <enet/enet.h>
#include "ComputerResourcesHelper.h"
#include "Main.h"

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	std::cout << std::endl;
	system("pause");
}

int main()
{
	enet_initialize();

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwSetErrorCallback(error_callback);
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Penguin Basket", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();

	#ifdef _WIN32
	// Turn on vertical screen sync under Windows.
	// (I.e. it uses the WGL_EXT_swap_control extension)
	typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(1);
	#endif

	unsigned int start = clock();
	Main* main = new Main(window);
	printf("Total init time: %d ms. \n", clock() - start);

	int frames = 0;
	double lastFrame = glfwGetTime();
	double deltaTime = 0;
	double lastUpdate = glfwGetTime();
	
	main->LoadResources();
	main->Init(window);

	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		frames++;
		deltaTime = glfwGetTime() - lastUpdate;
		lastUpdate = glfwGetTime();
		if (glfwGetTime() - lastFrame >= 1.0)
		{
			Settings::FPS = frames;
			Settings::RamUsage = ComputerResourcesHelper::GetRamUsage();
			printf("FPS: %d\n", frames);
			lastFrame = glfwGetTime();
			frames = 0;
		}

		main->Update(window, (float)deltaTime);
		main->Render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	glfwTerminate();

	if (Settings::Multiplayer) {
		if (main->m_Client != NULL)
			main->m_Client->Disconnect();
		enet_deinitialize();
	}

	return 0;
}