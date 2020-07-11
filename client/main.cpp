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

void HandleDirectionKeys(SDL_Keycode sym, int& r, int& c, int monsterPosR, int monsterPosC, bool preventOverlap)
{
	//Select surfaces based on key press
	switch (sym)
	{
	case SDLK_UP:
		if (r > 0 && (preventOverlap == false || (r - 1 != monsterPosR || c != monsterPosC)))
		{
			r--;
		}
		break;

	case SDLK_DOWN:
		if (r < 4 && (preventOverlap == false || (r + 1 != monsterPosR || c != monsterPosC)))
		{
			r++;
		}
		break;

	case SDLK_LEFT:
		if (c > 0 && (preventOverlap == false || (c - 1 != monsterPosC || r != monsterPosR)))
		{
			c--;
		}
		break;

	case SDLK_RIGHT:
		if (c < 9 && (preventOverlap == false || (c + 1 != monsterPosC || r != monsterPosR)))
		{
			c++;
		}
		break;
	}
}

static int hostgetversion(lua_State* l) {
	lua_pushnumber(l, 1);
	lua_pushnumber(l, 19);
	lua_pushnumber(l, 85);
	return 3;
}

static int HostLoadBmp(lua_State* l) {
	const char* filename = lua_tostring(l, 1);
	printf("HostLoadBmp: %s\n", filename);
	SDL_Surface* surface = SDL_LoadBMP(filename);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF));
	lua_pushlightuserdata(l, surface);
	return 1;
}

static int HostBlitTileObject(lua_State* l)
{
	SDL_Surface* screenSurface = (SDL_Surface*)lua_touserdata(l, 1);
	SDL_Surface* tileSurface = (SDL_Surface*)lua_touserdata(l, 2);
	int r = (int)lua_tointeger(l, 3);
	int c = (int)lua_tointeger(l, 4);
	SDL_Rect destRect = { 64 * c, 64 * r, 64, 64 };
	SDL_BlitSurface(tileSurface, NULL, screenSurface, &destRect);
	return 0;
}

int main(int argc, char* argv[])
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	printf("Current working dir: %s\n", buff);

	lua_State* l = luaL_newstate();
	luaL_openlibs(l);
	lua_register(l, "hostgetversion", hostgetversion);
	lua_register(l, "HostLoadBmp", HostLoadBmp);
	lua_register(l, "HostBlitTileObject", HostBlitTileObject);
	luaL_dofile(l, "mood.lua");

	lua_getglobal(l, "mood");
	lua_pushboolean(l, 1);
	lua_call(l, 1, 1);
	printf("The mood is %s\n", lua_tostring(l, -1));
	lua_pop(l, 1);

	luaL_dostring(l, "print('hohoho')");

	luaL_dofile(l, "hierarchy.lua");

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* gWindow = NULL;
	gWindow = SDL_CreateWindow("아름다운 세계", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Surface* gScreenSurface = SDL_GetWindowSurface(gWindow);

	lua_getglobal(l, "setscreensurface");
	lua_pushlightuserdata(l, gScreenSurface);
	lua_call(l, 1, 0);

	SDL_Surface* grassTileSurface = SDL_LoadBMP("logo.bmp");
	SDL_Surface* meTileSurface = SDL_LoadBMP("me.bmp");
	SDL_Surface* monsterTileSurface = SDL_LoadBMP("monster.bmp");
	SDL_Surface* numbersSurface = SDL_LoadBMP("numbers.bmp");
	SDL_Surface* selectSurface = SDL_LoadBMP("select.bmp");

	SDL_SetColorKey(meTileSurface, SDL_TRUE, SDL_MapRGB(meTileSurface->format, 0xFF, 0x00, 0xFF));
	SDL_SetColorKey(monsterTileSurface, SDL_TRUE, SDL_MapRGB(monsterTileSurface->format, 0xFF, 0x00, 0xFF));
	SDL_SetColorKey(numbersSurface, SDL_TRUE, SDL_MapRGB(numbersSurface->format, 0xFF, 0x00, 0xFF));
	SDL_SetColorKey(selectSurface, SDL_TRUE, SDL_MapRGB(selectSurface->format, 0xFF, 0x00, 0xFF));

	bool quit = false;

	int mePosC = 0;
	int mePosR = 0;
	int cursorPosC = 0;
	int cursorPosR = 0;

	int monsterPosC = 9;
	int monsterPosR = 2;

	int dirMode = 0;

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
				if (dirMode == 0)
				{
					HandleDirectionKeys(e.key.keysym.sym, mePosR, mePosC, monsterPosR, monsterPosC, true);
				}
				else
				{
					HandleDirectionKeys(e.key.keysym.sym, cursorPosR, cursorPosC, monsterPosR, monsterPosC, false);
				}

				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;

				case SDLK_d:
					dirMode = dirMode == 0;
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
		meDestRect.x = 64 * mePosC;
		meDestRect.y = 64 * mePosR;
		meDestRect.w = 64;
		meDestRect.h = 64;
		SDL_BlitSurface(meTileSurface, NULL, gScreenSurface, &meDestRect);

		SDL_Rect monsterDestRect;
		monsterDestRect.x = 64 * monsterPosC;
		monsterDestRect.y = 64 * monsterPosR;
		monsterDestRect.w = 64;
		monsterDestRect.h = 64;
		SDL_BlitSurface(monsterTileSurface, NULL, gScreenSurface, &monsterDestRect);

		SDL_Rect selectDestRect = { 64 * cursorPosC, 64 * cursorPosR, 64, 64 };
		SDL_BlitSurface(selectSurface, NULL, gScreenSurface, &selectDestRect);

		lua_getglobal(l, "gettileobjects");
		lua_pushboolean(l, 1);
		lua_call(l, 1, 1);
		//printf("gettileobjects returns '%s'\n", lua_tostring(l, -1));
		lua_pop(l, 1);

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
	SDL_FreeSurface(selectSurface);
	selectSurface = NULL;
	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();

	lua_close(l);

	return 0;
}
