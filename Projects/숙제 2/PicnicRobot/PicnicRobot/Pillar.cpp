#include "Pillar.h"
#include "ingame.h"


Pillar::Pillar(){

}

Pillar::Pillar(float in_x, float in_z) : x(in_x), z(in_z){
	y = PILLAR_HEIGHT;
}

void Pillar::Draw(){
	glPushMatrix();
	glTranslatef(x, y - (WINDOW_SIZE_Y / 2), z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glColor4f(0.25f, 0.125f, 0.f, 1.f);
	glutSolidCylinder(PILLAR_CIRCLE_RADIUS, y, 30, 1);
	glColor4f(0.f, 0.f, 0.f, 1.f);
	if (select_front_view)	glColor4f(1.f, 0.f, 0.f, 1.f);
	glutWireCylinder(PILLAR_CIRCLE_RADIUS + 1, y, 30, 1);
	
	if (top_view) {
		if(top_view_draw_collision || click_mouse_collision)		glColor4f(1.f, 0.f, 0.f, 1.f);
		else glColor4f(1.f, 1.f, 0.f, 1.f);
		glutWireCylinder(PILLAR_CIRCLE_RADIUS + PILLAR_BUILD_ACCESS, 2, 30, 1);
	}
	glPopMatrix();
}

void Pillar::Update(){
	
}




