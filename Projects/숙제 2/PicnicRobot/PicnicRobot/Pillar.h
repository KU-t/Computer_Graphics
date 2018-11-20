#pragma once


class Pillar{
public:
	float x, y, z;
	float circle_rad;
	bool select_mouse = false;
	bool top_view = false;
	bool top_view_draw_collision = false;
	bool click_mouse_collision = false;

	Pillar();
	Pillar(float in_x, float in_z);
	void Draw();
	void Update();
};

