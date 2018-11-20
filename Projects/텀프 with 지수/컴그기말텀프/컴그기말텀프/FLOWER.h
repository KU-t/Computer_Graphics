#pragma once
#include <random>
class FLOWER {
public:
	float x, y, z;
	float rad = 0.f;
	float rotation = 0.f;
	float collision_size = 90;
	float r = 1.0f - (rand() % 5) * 0.1;
	float g = 1.0f - (rand() % 5) * 0.1;
	float b = 1.0f - (rand() % 5) * 0.1;

	FLOWER();
	FLOWER(float in_x, float in_y, float in_z);
	~FLOWER();

	void Draw();
	void Draw_Collision_rect();
	void Update();
};

