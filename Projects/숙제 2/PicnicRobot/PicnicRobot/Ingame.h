#pragma once

#include <gl/freeglut.h>
#include "Ground.h"

#define WIDNOW_SIZE_X 800
#define WIDNOW_SIZE_Y 800
#define WIDNOW_SIZE_Z 800

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
void Change_Scene(SCENE in_scene, float in_move_x, float in_move_y, float in_move_z, float in_Angle_x_radian, VIEW in_view);


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