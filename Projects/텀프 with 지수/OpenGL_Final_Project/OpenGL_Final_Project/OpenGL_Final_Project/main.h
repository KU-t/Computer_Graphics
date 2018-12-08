#pragma once
//Include
#include <gl/freeglut.h>
#include <time.h>
#include <random>
#include <math.h>
#include <iostream>
#include <random>
#include <fstream>
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")


#include "PUMPKIN.h"
#include "WATERMELLON.h"
#include "TANGERINE.h"
#include "APPLE.h"
#include "TOMATO.h"
#include "GRAPE.h"
#include "BALL.h"
#include "Rect.h"
#include "ITEM.h"

#include "Particle.h"

using namespace std;

//Define
#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 800
#define WINDOW_SIZE_Z 800
#define MAP_Z_SIZE 10000
#define MAX_ICE 100
#define LIGHT_X 0
#define LIGHT_Y 100
#define LIGHT_Z 150
#define deg(x) 3.141592 * x / 180

// 충돌체크 사이즈
#define PUMP_COLL 90
#define APP_COLL 60
#define TANGER_COLL 70
#define TOMA_COLL 75
#define WATERM_COLL 105
#define GRAPE_COLL 75
#define ITEM_COLL 75

// 객체 개수
#define MAX_GRAPE 100
#define MAX_APP 100
#define MAX_TANGER 100
#define MAX_TOMA 100
#define MAX_WATERM 100
#define MAX_PUMP 100
#define MAX_BALL 100
#define MAX_PARTICLE 100
#define MAX_RECT 100
#define MAX_ITEM 100

// 볼 속성
#define BALL_SIZE 30
#define BALL_INIT_Y_VEL 50
#define BALL_INIT_Z_VEL 200


// 전체속성
#define CREATE_HUDDLE_SPEED 45	// 15의 배수 // 장애물 나오는 속도
#define CREATE_HUDDLE_Z -8000
#define DELETE_HUDDLE_Z 0
#define DELETE_BALL_Z -8000
#define DELETE_PARTICLE_Z 1000
#define DELETE_RECT_Z 0
#define GRAVITY -9.8
#define RECT_SIZE 200
#define TIME_MULTIBALL 300
#define ADD_TIME_MULTIBALL 300

// 장애물 이동 속도
#define HUDDLE_MOVE_SPEED_1 30
#define HUDDLE_MOVE_SPEED_2 45
#define HUDDLE_MOVE_SPEED_3 60
#define HUDDLE_MOVE_SPEED_4 75
#define HUDDLE_MOVE_SPEED_5 100

// 장애물 속도 결정 score
#define HUDDLE_SCORE_1 1000
#define HUDDLE_SCORE_2 2000
#define HUDDLE_SCORE_3 3000
#define HUDDLE_SCORE_4 4000
#define HUDDLE_SCORE_5 5000

#define HUDDLE_WAVE_MOVE 10

#define START_STATE_INITE_NUMBER 0

//particle


//Enum
enum VIEW { Perspective, Orthographic };
enum OBJECT{Apple = 1, Grape = 2, Pumkin = 3, Tangerine = 4, Tomato = 5, Watermellon = 6, Ball = 7, Particle = 8, NONE = 9, Rect = 10, Item = 11};
enum STATE {START_STATE, PLAY_STATE, END_STATE};

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
void Draw_bottom();
void Add_Object(float in_x, float in_y, float in_z, OBJECT object, int in_moving, float in_data_min, float in_data_max, float in_r = 0, float in_g = 0, float in_b = 0, bool type = false);
int FindslotObject(OBJECT object);
void Draw_Objects();
void Update_Objects(STATE curr_state);
void Read_txt();
bool Collision_check(float bx, float by, float bz, float bsize, float hx, float hy, float hz, float hsize);
bool Collision_Rect_check(float bx, float by, float bz, float hx, float hy, float hz);
void Collision_Object();
void drawText(const char *text, int length, int x, int y);
void Check_Update_Huddle_Speed();
void Click_Draw();
void Check_Game_Over();
void Write_FRUIT_CRUSH(float x, float y, float z);
void Write_GAME_OVER(float x, float y, float z);
void Draw_End_state_Background(float move_z);
