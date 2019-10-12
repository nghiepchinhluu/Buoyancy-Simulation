#pragma once


namespace bird
{
	struct bird
	{
		int Size;
		int Position;
		float Velocity;
		float Acceleration;
	};


	void initBird(bird* Bird)
	{
		Bird->Size = 50;
		Bird->Position = Screen_Height / 2;
		Bird->Velocity = -70;
		Bird->Acceleration = 15;
	}

	void updatePos(bird* Bird)
	{
		Bird->Velocity += Bird->Acceleration * 0.4;
		Bird->Position += Bird->Velocity * 0.4;
	}
}