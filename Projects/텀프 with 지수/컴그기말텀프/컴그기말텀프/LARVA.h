#pragma once
#include <random>
class LARVA {
public:
	float x, y, z;
	float rad = 0.f;
	float rotation = 0.f;
	float collision_size = 90;


	LARVA();
	LARVA(float in_x, float in_y, float in_z);
	~LARVA();

	void Draw();
	void Draw_Collision_rect();
	void Update();
};

