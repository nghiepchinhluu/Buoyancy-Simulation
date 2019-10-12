#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#pragma warning (disable:4996)

#pragma comment(linker,"/subsystem:console")

// Include SDL lib
#include "Headers/SDL2-2.0.8/include/SDL.h"
#include "Headers/SDL2-2.0.8/include/SDL_image.h"
#include "Headers/SDL2-2.0.8/include/SDL_mixer.h"
#pragma comment(lib,"Headers\\SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"Headers\\SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"Headers\\SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"Headers\\SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"Headers\\SDL2-2.0.8\\lib\\x86\\SDL2_mixer.lib")


SDL_Window* window;
int screen_width = 1200;
int screen_height = 800;

struct Object
{
	float x, y;
	float vx, vy;
	float w, h;
	float m;
	float ax, ay;
};

float g = 500.0;
float d_object = 334.0;
float d_water = 1000.0;
float water_friction = 3.0;

void Load_Images(SDL_Texture** sky_texture, SDL_Texture** ocean_texture, SDL_Texture** object_texture, const char* sky_file, const char* ocean_file, const char* object_file, SDL_Renderer* renderer)
{
	SDL_Surface* sky_surface = IMG_Load(sky_file);
	*sky_texture = SDL_CreateTextureFromSurface(renderer,sky_surface);
	SDL_Surface* ocean_surface = IMG_Load(ocean_file);
	*ocean_texture = SDL_CreateTextureFromSurface(renderer,ocean_surface);
	SDL_Surface* object_surface = IMG_Load(object_file);
	*object_texture = SDL_CreateTextureFromSurface(renderer, object_surface);
}

void InitializeObject(Object* object)
{
	object->m = object->w*1*object->h * d_object; // m=V.density
	object->vx = 0.0;
	object->vy = 0.0;
	object->ax = 0.0;
	object->ay = g;
}

void RenderObjectList(const Object* object_list, const int n_objects, SDL_Texture* object_texture, SDL_Renderer* renderer)
{
	for (int i = 0; i < n_objects; i++)
	{
		SDL_Rect object_box = { object_list[i].x,object_list[i].y,object_list[i].w,object_list[i].h };
		SDL_RenderCopyEx(renderer, object_texture, NULL, &object_box, NULL, NULL, SDL_FLIP_NONE);
	}
}

void UpdatePhysics(Object* object_list, bool* spawned, const int n_objects, float t)
{
	// Unit of t: second(s)
	for (int i = 0; i < n_objects; i++)
	{
		if (spawned[i] == false) continue;
		float full_volume = object_list[i].w * 1* object_list[i].h;
		float distance_underwater = object_list[i].y + object_list[i].h - screen_height/2;
		float volume_underwater = distance_underwater * object_list[i].w * 1;
		if (volume_underwater > full_volume) volume_underwater = full_volume;
		if (distance_underwater > 0)
		{
			object_list[i].ay = g - g * d_water * volume_underwater / object_list[i].m - water_friction * object_list[i].vy;
		}
		else
		{
			object_list[i].ay = g;
		}

		object_list[i].x += object_list[i].vx*t;
		object_list[i].y += object_list[i].vy*t;
		object_list[i].vx += object_list[i].ax*t;
		object_list[i].vy += object_list[i].ay*t;
	}
}

int main(int argc, char** argv)
{
	// Init
	HWND windowHandle = GetConsoleWindow();
	//ShowWindow(windowHandle, SW_HIDE);
	srand(time(0));
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("WaterSimulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer= SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* sky_texture = NULL, *ocean_texture = NULL, *object_texture = NULL;
	Load_Images(&sky_texture, &ocean_texture, &object_texture, "Images/sky.jpg", "Images/ocean.jpg", "Images/object.jpg",renderer);
	Object* object_list = (Object*)malloc(sizeof(Object) * 200);
	bool* spawned = (bool*)malloc(sizeof(bool) * 200);
	int n_objects = 0;
	bool hold = false;
	int save_x, save_y;
	int last_sizeincrease = clock();
	int last_physicsupdate = clock();
	for (;;)
	{
		SDL_Event event;
		int mx = 0, my = 0;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 0;
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					hold = true;
					memset(&object_list[n_objects], 0, sizeof(Object));
					object_list[n_objects].x = event.button.x;
					object_list[n_objects].y = event.button.y;
					save_x = object_list[n_objects].x;
					save_y = object_list[n_objects].y;
					spawned[n_objects] = false;
					n_objects++;
				}
			}
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				hold = false;
				InitializeObject(&object_list[n_objects - 1]);
				spawned[n_objects -1] = true;
			}
		}
		// Update
		int current_clock = clock();
		if (hold && current_clock - last_sizeincrease > 100)
		{
			// For spawning objects
			int x, y;
			SDL_GetMouseState(&x, &y);
			if (x > save_x)
			{
				object_list[n_objects - 1].w = x - save_x;
				object_list[n_objects - 1].x = save_x;
			}
			else
			{
				object_list[n_objects - 1].w = save_x - x;
				object_list[n_objects - 1].x = x;
			}
			if (y > save_y)
			{
				object_list[n_objects - 1].h = y - save_y;
				object_list[n_objects - 1].y = save_y;
			}
			else
			{
				object_list[n_objects - 1].h = save_y - y;
				object_list[n_objects - 1].y = y;
			}
			last_sizeincrease = current_clock;
		}
		current_clock = clock();
		if (current_clock - last_physicsupdate > 1)
		{
			UpdatePhysics(object_list, spawned, n_objects, float(current_clock - last_physicsupdate) / 1000);
			last_physicsupdate = current_clock;
		}

		// Render
		SDL_Rect sky_box = { 0,0,screen_width,screen_height/2 };
		SDL_Rect ocean_box = { 0,screen_height / 2,screen_width,screen_height / 2 };
	
		SDL_RenderCopyEx(renderer, sky_texture, NULL, &sky_box, NULL, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, ocean_texture, NULL, &ocean_box, NULL, NULL, SDL_FLIP_NONE);
		RenderObjectList(object_list, n_objects, object_texture, renderer);
		SDL_RenderPresent(renderer);
	}
	return 0;
}