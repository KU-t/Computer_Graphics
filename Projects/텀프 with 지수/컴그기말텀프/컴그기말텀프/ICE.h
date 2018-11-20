#pragma once
#include <random>
class ICE {
public:
	float x, y, z;
	float rad = 0.f;
	float r = 1.0f - (rand() % 5) * 0.1;
	float g = 1.0f - (rand() % 5) * 0.1;
	float b = 1.0f - (rand() % 5) * 0.1;
	float rotation = 0.f;

	float collision_size = 90;

	ICE();
	ICE(float in_x, float in_y, float in_z);
	~ICE();

	void Draw();
	void Draw_Collision_rect();
	void Update();
};

