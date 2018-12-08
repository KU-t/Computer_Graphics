#pragma once
#include "main.h"


class PUMPKIN {
public:
	float x, y, z;
	float rad = 0.f;
	float rotation = 0.f;
	float size = 60.f;
	float data_min, data_max;
	int socre = 50;

	int moving;
	int data;

	PUMPKIN();
	PUMPKIN(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max);
	~PUMPKIN();

	void Draw();
	void Update(float Huddle_speed);
};

