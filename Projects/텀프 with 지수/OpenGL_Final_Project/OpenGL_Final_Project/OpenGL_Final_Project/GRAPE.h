#pragma once
#include "main.h"


class GRAPE {
public:
public:
	float x, y, z;
	float rad = 0.f;
	float size = 30.f;
	float rotation = 0.f;
	float data_min, data_max;
	int socre = 100;

	int moving;
	int data;

	void Draw();
	void Update(float Huddle_speed);

	GRAPE();
	GRAPE(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max);
	~GRAPE();
};
