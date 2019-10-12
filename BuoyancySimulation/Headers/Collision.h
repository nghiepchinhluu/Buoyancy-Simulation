#pragma once


namespace collision
{
	bool inside(int x, int y, SDL_Rect box)
	{
		if (x < box.x) return false;
		if (x > box.x + box.w) return false;
		if (y < box.y) return false;
		if (y > box.y + box.h) return false;
		return true;
	}

	bool Collide(bird::bird Bird, SDL_Rect* Pipes, int Npipes)
	{
		SDL_Rect BirdCapture = { 310,Bird.Position + 12,30,15 };
		for (int i = 0; i < Npipes; i++)
		{
			SDL_Rect PipeCapture = { Pipes[i].x,Pipes[i].y,Pipes[i].w,Pipes[i].h };
			if (inside(BirdCapture.x, BirdCapture.y, PipeCapture)) return true;
			if (inside(BirdCapture.x + BirdCapture.w, BirdCapture.y, PipeCapture)) return true;
			if (inside(BirdCapture.x, BirdCapture.y + BirdCapture.h, PipeCapture)) return true;
			if (inside(BirdCapture.x + BirdCapture.w, BirdCapture.y + BirdCapture.h, PipeCapture)) return true;
		}
		if (Bird.Position > Screen_Height) return true;
		return false;
	}
}
