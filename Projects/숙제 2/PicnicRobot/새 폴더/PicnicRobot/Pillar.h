#pragma once
#define PILLAR_BUILD_ACCESS 100.f
#define PILLAR_ROCK_ACCESS 30.f

class Pillar{
public:
	float x, y, z;
	bool top_view = false;
	bool select_front_view = false;
	bool top_view_draw_collision = false;
	bool click_mouse_collision = false;
	bool rock = false;
	float access_size = PILLAR_BUILD_ACCESS;

	Pillar();
	Pillar(float in_x, float in_z, bool rock);
	void Draw();
	void Update();
};

