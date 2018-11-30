#pragma once
#include "ingame.h"

class Bullet{
public:
	float x, y, z;
	float dir;
	Bullet();
	Bullet(float in_x, float in_y, float in_z, float rad);
	~Bullet();
	void Draw();
	void Update();
};

