#pragma once

//Include
#include <gl/freeglut.h>
#include <time.h>
#include <random>
#include <math.h>
#include <iostream>

#include "ICE.h"
#include "DONUT.h"
#include "LARVA.h"
#include "FLOWER.h"

using namespace std;

//Define
#define W_x 800
#define W_y 800
#define W_z 800
#define MAP_Z_SIZE 10000
#define MAX_ICE 100
#define LIGHT_X 0
#define LIGHT_Y 100
#define LIGHT_Z 150
#define deg(x) 3.141592 * x / 180

//Enum
enum VIEW { Perspective, Orthographic };

//Class
class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
};

//Funtion
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);
void Change_Angle_xyz();
void Change_View(VIEW view);
void Draw_Coordinates();
void Draw_bottom();
void Add_BEE(float in_x, float in_y, float in_z);
int FindslotBee();
