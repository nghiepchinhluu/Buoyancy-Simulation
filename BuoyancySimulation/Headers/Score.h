#pragma once

namespace score
{
	int score(SDL_Rect* Pipes, int Npipes, int Birdpos)
	{
		for (int i = 0; i < Npipes; i++)
		{
			if ((Pipes[i].x < Birdpos) && (Pipes[i].x + 10 >= Birdpos)) return 1;
		}
		return 0;
	}

	void showScore(int Score, SDL_Texture* ScoreSheet_Text, SDL_Renderer* renderer)
	{
		int tmp = Score;
		int Ndigit = 0;
		while (tmp > 0)
		{
			tmp /= 10;
			Ndigit++;
		}
		SDL_Rect DestRect = { 0,150,40,50 };
		int Start = (Screen_Width + Ndigit * DestRect.w) / 2;
		Start -= DestRect.w;
		DestRect.x = Start;
		while (Score > 0)
		{
			int tmp = Score % 10;
			SDL_Rect SrcRect = { tmp * 50,0,50,49 };
			SDL_RenderCopyEx(renderer, ScoreSheet_Text, &SrcRect, &DestRect, 0, 0, SDL_FLIP_NONE);
			Score /= 10;
			DestRect.x -= 40;
		}
	}
}