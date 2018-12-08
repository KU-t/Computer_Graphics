#pragma once
#include "main.h"

#define MAX_PARTICLE_PART 100
#define MAX_INIT_VEL 70
#define HALF_MAX_INIT_VEL MAX_INIT_VEL * 0.5f
#define PARTICLE_SIZE 5

class Point {
public:
	float x, y, z;
	float vel_x, vel_y, vel_z;

	Point();
	void Init(float x, float y, float z, float vel_x, float vel_y, float vel_z);
	void Update();
};

class PARTICLE {
public:
	float x, y, z;
	float r = 0.f, g = 0.f, b = 0.f;
	float speed;
	Point p[MAX_PARTICLE_PART];

	PARTICLE();
	PARTICLE(float x, float y, float z, float r, float g, float b);
	~PARTICLE();
	void Draw();
	void Update();
};
