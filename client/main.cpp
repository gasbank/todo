// C++
#include <iostream>
#include <string>
#include <vector>

#include <lua.hpp>

// SDL
#include <SDL.h>
#include <glad/glad.h>

// Dear ImGui
//#include "imgui-style.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

#include <stdio.h>  /* defines FILENAME_MAX */
//#define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#include "main.h"
#define GetCurrentDir getcwd
#endif

int windowWidth = 1280, windowHeight = 720;

char buf3[1024];

static int hostgetversion(lua_State* l) {
	lua_pushnumber(l, 1);
	lua_pushnumber(l, 19);
	lua_pushnumber(l, 85);
	return 3;
}

void ShowLuaConsole(bool* p_open, lua_State* l);
int grpcworld_main(int argc, char** argv);
int grpcworld_todolist_main(int argc, char** argv);

void BlitNumbers(Uint32 number, SDL_Surface* numbersSurface, SDL_Surface* gScreenSurface)
{
	auto nstr = std::to_string(number);
	int i = 0;
	for (auto c : nstr)
	{
		auto idx = c - '0';
		if (idx >= 0 && idx <= 9)
		{
			SDL_Rect numberSrcRect = { 32 * idx, 0, 32, 32 };
			SDL_Rect numberDestRect = { 32 * i, 0, 32, 32 };
			SDL_BlitSurface(numbersSurface, &numberSrcRect, gScreenSurface, &numberDestRect);
			i++;
		}
	}
}

int main(int argc, char* argv[])
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	printf("Current working dir: %s\n", buff);

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* gWindow = NULL;
	gWindow = SDL_CreateWindow("아름다운 세계", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Surface* gScreenSurface = SDL_GetWindowSurface(gWindow);

	SDL_Surface* grassTileSurface = SDL_LoadBMP("logo.bmp");
	SDL_Surface* meTileSurface = SDL_LoadBMP("me.bmp");
	SDL_Surface* monsterTileSurface = SDL_LoadBMP("monster.bmp");
	SDL_Surface* numbersSurface = SDL_LoadBMP("numbers.bmp");

	SDL_SetColorKey(meTileSurface, SDL_TRUE, SDL_MapRGB(meTileSurface->format, 0xFF, 0x00, 0xFF));
	SDL_SetColorKey(monsterTileSurface, SDL_TRUE, SDL_MapRGB(monsterTileSurface->format, 0xFF, 0x00, 0xFF));
	SDL_SetColorKey(numbersSurface, SDL_TRUE, SDL_MapRGB(numbersSurface->format, 0xFF, 0x00, 0xFF));

	bool quit = false;

	int mePosC = 0;
	int mePosR = 0;

	int monsterPosC = 9;
	int monsterPosR = 2;

	//Event handler
	SDL_Event e;
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					if (mePosR - 1 != monsterPosR || mePosC != monsterPosC)
					{
						mePosR--;
					}
					break;

				case SDLK_DOWN:
					if (mePosR + 1 != monsterPosR || mePosC != monsterPosC)
					{
						mePosR++;
					}
					break;

				case SDLK_LEFT:
					if (mePosC - 1 != monsterPosC || mePosR != monsterPosR)
					{
						mePosC--;
					}
					break;

				case SDLK_RIGHT:
					if (mePosC + 1 != monsterPosC || mePosR != monsterPosR)
					{
						mePosC++;
					}
					break;

				case SDLK_ESCAPE:
					quit = true;
					break;

				default:
					break;
				}
			}
		}

		for (int r = 0; r < 5; r++)
		{
			for (int c = 0; c < 10; c++)
			{
				SDL_Rect destRect;
				destRect.w = grassTileSurface->clip_rect.w;
				destRect.h = grassTileSurface->clip_rect.h;
				destRect.x = c * grassTileSurface->clip_rect.w;
				destRect.y = r * grassTileSurface->clip_rect.h;
				SDL_BlitSurface(grassTileSurface, NULL, gScreenSurface, &destRect);
			}
		}

		SDL_Rect meDestRect;
		meDestRect.w = 64;
		meDestRect.h = 64;
		meDestRect.x = 64 * mePosC;
		meDestRect.y = 64 * mePosR;
		SDL_BlitSurface(meTileSurface, NULL, gScreenSurface, &meDestRect);

		SDL_Rect monsterDestRect;
		monsterDestRect.w = 64;
		monsterDestRect.h = 64;
		monsterDestRect.x = 64 * monsterPosC;
		monsterDestRect.y = 64 * monsterPosR;
		SDL_BlitSurface(monsterTileSurface, NULL, gScreenSurface, &monsterDestRect);

		BlitNumbers(SDL_GetTicks(), numbersSurface, gScreenSurface);

		SDL_UpdateWindowSurface(gWindow);
	}
	SDL_FreeSurface(grassTileSurface);
	grassTileSurface = NULL;
	SDL_FreeSurface(meTileSurface);
	meTileSurface = NULL;
	SDL_FreeSurface(monsterTileSurface);
	monsterTileSurface = NULL;
	SDL_FreeSurface(numbersSurface);
	numbersSurface = NULL;
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
	return 0;
}

int main_imgui_test(int argc, char* argv[]) {

	//grpcworld_main(argc, argv);

	//grpcworld_todolist_main(argc, argv);

	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	printf("Current working dir: %s\n", buff);

	lua_State* l = luaL_newstate();
	luaL_openlibs(l);
	lua_register(l, "hostgetversion", hostgetversion);
	luaL_dofile(l, "mood.lua");

	lua_getglobal(l, "mood");
	lua_pushboolean(l, 1);
	lua_call(l, 1, 1);
	printf("The mood is %s\n", lua_tostring(l, -1));
	lua_pop(l, 1);

	luaL_dostring(l, "print('hohoho')");

	// initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("[ERROR] %s\n", SDL_GetError());
		return -1;
	}

	// set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	std::string glsl_version = "";
#ifdef __APPLE__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 150";
	SDL_GL_SetAttribute( // required on Mac OS
		SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#elif __linux__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
	// GL 3.0 + GLSL 130
	glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("NetworkGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, window_flags);
	// limit to which minimum size user can resize the window
	SDL_SetWindowMinimumSize(window, 500, 300);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);

	// enable VSync
	SDL_GL_SetSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "[ERROR] Couldn't initialize glad" << std::endl;
	}
	else {
		std::cout << "[INFO] glad initialized\n";
	}

	glViewport(0, 0, windowWidth, windowHeight);

	// setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.Fonts->AddFontFromFileTTF("Hack-Regular.ttf", 18.0f, NULL, NULL);

	// setup Dear ImGui style
	ImGui::StyleColorsDark();

	// setup platform/renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());

	// colors are set in RGBA, but as float
	ImVec4 background = ImVec4(35 / 255.0f, 35 / 255.0f, 35 / 255.0f, 1.00f);

	glClearColor(background.x, background.y, background.z, background.w);
	bool loop = true;
	while (loop) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// without it you won't have keyboard input and other things
			ImGui_ImplSDL2_ProcessEvent(&event);
			// you might also want to check io.WantCaptureMouse and
			// io.WantCaptureKeyboard before processing events

			switch (event.type) {
			case SDL_QUIT:
				loop = false;
				break;

			case SDL_WINDOWEVENT:
				switch (event.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					windowWidth = event.window.data1;
					windowHeight = event.window.data2;
					// std::cout << "[INFO] Window size: "
					//           << windowWidth
					//           << "x"
					//           << windowHeight
					//           << std::endl;
					glViewport(0, 0, windowWidth, windowHeight);
					break;
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					loop = false;
					break;
				}
				break;
			}
		}

		// start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		// a window is defined by Begin/End pair
		{
			static int counter = 0;
			// get the window size as a base for calculating widgets geometry
			int sdl_width = 0, sdl_height = 0, controls_width = 0;
			SDL_GetWindowSize(window, &sdl_width, &sdl_height);
			controls_width = sdl_width;
			// make controls widget width to be 1/3 of the main window width
			if ((controls_width /= 3) < 300) {
				controls_width = 300;
			}

			// position the controls widget in the top-right corner with some margin
			ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
			// here we set the calculated width and also make the height to be
			// be the height of the main window also with some margin
			ImGui::SetNextWindowSize(ImVec2(static_cast<float>(controls_width),
				static_cast<float>(sdl_height - 20)),
				ImGuiCond_Always);
			// create a window and append into it
			ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);

			ImGui::Dummy(ImVec2(0.0f, 1.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
			ImGui::Text("%s", SDL_GetPlatform());
			ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
			ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
			ImGui::InputText("##text3", buf3, 1024);

			// buttons and most other widgets return true when
			// clicked/edited/activated
			if (ImGui::Button("Counter button")) {
				std::cout << "counter button clicked\n";
				counter++;
			}
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::End();
		}

		static bool console_open = true;

		ShowLuaConsole(&console_open, l);

		// rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	ImGui_ImplOpenGL3_Shutdown();

	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	lua_close(l);

	return 0;
}
