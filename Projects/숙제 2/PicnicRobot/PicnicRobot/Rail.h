#pragma once
#include "Ingame.h"

#define MAX_TUNNEL 50


class Rail{
public:

	int draw_Rollercoaster_pillar_num[5] = { 0,0,0,0,0 };
	int draw_Rollercoaster_pillar_t[5] = {24, 18, 12, 6,0};
	
	int Rollercoaster_pillar_num[5] = {0,0,0,0,0};
	int Rollercoaster_pillar_t[5] = {20, 15, 10, 5,0};
	
	float x[5], y[5], z[5];
	float spline_y[5], spline_z[5];
	
	float tunnel_x[MAX_TUNNEL], tunnel_y[MAX_TUNNEL], tunnel_z[MAX_TUNNEL];
	float tunnel_spline_y[MAX_TUNNEL], tunnel_spline_z[MAX_TUNNEL];

	Rail();
	~Rail();

	void Draw();
};

