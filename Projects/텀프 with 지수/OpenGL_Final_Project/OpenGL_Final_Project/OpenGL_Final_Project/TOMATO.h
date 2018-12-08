#pragma once
#include "main.h"



class TOMATO {
public:
public:
	float x, y, z;
	float rad = 0.f;
	float size = 60.f;
	float rotation = 0.f;
	float data_min, data_max;
	int socre = 70;

	int moving;
	int data;

	void Draw();
	void Update(float Huddle_speed);

	TOMATO();
	TOMATO(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max);
	~TOMATO();
};
