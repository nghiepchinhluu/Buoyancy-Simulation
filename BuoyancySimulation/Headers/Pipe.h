#pragma once

namespace pipe
{
	void initPipe(SDL_Rect* Pipes, int Npipes)
	{
		int tmpPos = 1000;
		for (int i = 0; i < Npipes; i += 2)
		{
			Pipes[i].x = tmpPos;
			Pipes[i].y = 0;
			Pipes[i].w = 50;
			Pipes[i].h = rand() % (Screen_Height - 400) + 200;
			Pipes[i + 1].x = tmpPos;
			Pipes[i + 1].y = Pipes[i].h + 120;
			Pipes[i + 1].w = 50;
			Pipes[i + 1].h = Screen_Height - Pipes[i + 1].y;
			tmpPos += 350;
		}
	}

	void updatePipe(SDL_Rect* Pipes, int Npipes)
	{
		for (int i = 0; i < Npipes; i += 2)
		{
			Pipes[i].x -= 10;
			Pipes[i + 1].x -= 10;
			if (Pipes[i].x <= 0)
			{
				Pipes[i].x = 3500;
				Pipes[i].h = rand() % (Screen_Height - 400) + 200;
				Pipes[i + 1].x = 3500;
				Pipes[i + 1].y = Pipes[i].h + 120;
				Pipes[i + 1].h = Screen_Height - Pipes[i + 1].y;
			}
		}
	}

	void showPipe(SDL_Rect* Pipes, int Npipes, SDL_Texture* PipeBodySprite_Text, SDL_Texture* PipeHeadSprite_Text, SDL_Renderer *renderer)
	{
		for (int i = 0; i < Npipes; i += 2)
		{
			if (Pipes[i].x < Screen_Width)
			{
				SDL_Rect tmp = { Pipes[i].x, Pipes[i].y , Pipes[i].w, Pipes[i].h - 30 };
				SDL_RenderCopyEx(renderer, PipeBodySprite_Text, NULL, &tmp, 0, 0, SDL_FLIP_VERTICAL);
				tmp = { Pipes[i].x, Pipes[i].h-30 , Pipes[i].w, 30 };
				SDL_RenderCopyEx(renderer, PipeHeadSprite_Text, NULL, &tmp, 0, 0, SDL_FLIP_VERTICAL);
				tmp = { Pipes[i + 1].x, Pipes[i + 1].y + 30 , Pipes[i + 1].w, Pipes[i + 1].h - 30};
				SDL_RenderCopyEx(renderer, PipeBodySprite_Text, NULL, &tmp, 0, 0, SDL_FLIP_NONE);
				tmp = { Pipes[i + 1].x, Pipes[i + 1].y , Pipes[i + 1].w, 30 };
				SDL_RenderCopyEx(renderer, PipeHeadSprite_Text, NULL, &tmp, 0, 0, SDL_FLIP_NONE);
			}
		}
	}
}