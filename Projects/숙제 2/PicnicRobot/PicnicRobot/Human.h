#pragma once
#include "Ingame.h"
#define HUMAN_SPEED 5
#define HUMAN_SIZE 40

class Human{
public:
	float x, y, z;
	bool hand_foot = false;
	float rad_hand_foot = 0.f;
	float rad_move = 0.f;
	float speed = HUMAN_SPEED;

	Human();
	~Human();
	void Update();
	void Draw();
};

