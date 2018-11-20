#pragma once
#include <random>
class DONUT {
public:
	float x, y, z;
	float rad = 0.f;
	float rotation = 0.f;
	float collision_size = 90;

	int type = rand() % 3;

	DONUT();
	DONUT(float in_x, float in_y, float in_z);
	~DONUT();

	void Draw();
	void Draw_Collision_rect();
	void Update();
};

