#pragma once

#include <gl/freeglut.h>
#include <math.h>
#include <iostream>
#include "Ground.h"
#include "Pillar.h"
#include "Rail.h"
#include <time.h>

using namespace std;

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 800
#define WINDOW_SIZE_Z 800
#define deg(x) 3.141592*x/180
#define rad(x) 180 * x / 3.141592

#define MAX_PILLAR 30	
#define MAX_ROCK 10
#define MAX_ROCK_PILLAR MAX_PILLAR + MAX_ROCK

#define PILLAR_HEIGHT 50.f
#define PILLAR_CIRCLE_RADIUS 30.f


#define SPLINE_COUNT 100


enum VIEW { Perspective, Orthographic };
enum SCENE { TOP_VIEW, FRONT_VIEW, PLAY_VIEW };
enum SPEED {X1 = 1, X3 = 3, X5 = 5, X7 = 7};

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
bool Collision_New_Pillar(float px, float pz, float mx, float my, float in_access_size);
bool Collision_Pillar_Pillar(float p1_x, float p1_z, float p2_x, float p2_z, float p_access_size);
bool Every_Pillar_Not_Collision();
bool Every_Pillar_Not_Click_Collision();
void Draw_Spline(Pillar *P1, Pillar *P2, Pillar *P3, Pillar *P4, int i);
void Draw_Pillars_Spline();
void Draw_rollercoaster();
void Draw_tunnel();

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