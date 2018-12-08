#pragma once
#include "main.h"

class BALL{
public:
	float x, y, z;
	float vel_x, vel_y, vel_z;
	float rad = 0.f;
	bool type;

	BALL();
	BALL(float in_x, float in_y, bool in_type);
	~BALL();
	void Draw();
	void Update();
};

