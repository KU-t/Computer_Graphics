#pragma once
#include "Ingame.h"
#define HUMAN_SPEED 5
#define HUMAN_SIZE 40
#define MAX_Y_CAPE 20
#define RAD_CAPE_INCREASE 10
class Human{
public:
	float x, y, z;
	bool hand_foot = false;
	float rad_hand_foot = 0.f;
	float rad_move = 0.f;
	float speed = HUMAN_SPEED;
	bool choice;
	float p[4][3];
	int rad_cape = 0;

	Human();
	~Human();
	void Update();
	void Draw();
};

