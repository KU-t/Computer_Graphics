#pragma once

#include <gl/freeglut.h>
#include <math.h>
#include <iostream>
#include "Ground.h"
#include "Pillar.h"
#include "Rail.h"

using namespace std;

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 800
#define WINDOW_SIZE_Z 800
#define deg(x) 3.141592*x/180
#define rad(x) 180 * x / 3.141592

#define MAX_PILLAR 10
#define PILLAR_HEIGHT 50.f
#define PILLAR_CIRCLE_RADIUS 30.f
#define PILLAR_BUILD_ACCESS 100.f

#define SPLINE_COUNT 100

#define MAX_RAIL MAX_PILLAR * 100

enum VIEW { Perspective, Orthographic };
enum SCENE { TOP_VIEW, FRONT_VIEW, PLAY_VIEW };

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);
void Change_Angle_xyz();
void Change_View(VIEW view);
void Draw_Coordinates();
bool Collision_CIrcles(float x1, float y1, float r1, float x2, float y2, float r2);
void Draw_Objects();
void Change_Scene(SCENE in_scene, float in_move_x, float in_move_y, float in_move_z, float in_Angle_x_radian, VIEW in_view);
void Add_Pillar(float x, float z);
int Find_Pillar();
bool Collision_Pillar_mouse(SCENE scene, float px, float py, float mx, float my);
bool Collision_New_Pillar(float px, float pz, float mx, float my);
bool Collision_Pillar_Pillar(float p1_x, float p1_z, float p2_x, float p2_z);
bool Every_Pillar_Not_Collision();
bool Every_Pillar_Not_Click_Collision();
void Draw_Spline(Pillar *P1, Pillar *P2, Pillar *P3, Pillar *P4);
void Draw_Pillars_Spline();

class Angle {
public:
	float radian;
	bool sw;

	Angle() {
		radian = 0.f;
		sw = false;
	}
	Angle(float radian) : radian(radian){
		sw = false;
	}
};