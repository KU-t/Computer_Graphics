#include "Pillar.h"
#include "ingame.h"


Pillar::Pillar(){

}

Pillar::Pillar(float in_x, float in_z, bool in_rock) : x(in_x), z(in_z),rock(in_rock){
	y = PILLAR_HEIGHT;
	if (rock)		access_size = PILLAR_ROCK_ACCESS;
}

void Pillar::Draw(){
	glPushMatrix();
	glTranslatef(x, y - (WINDOW_SIZE_Y / 2), z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	
	if (rock) {
		glPushMatrix();
		glColor4f(0.3f, 0.3f, 0.3f, 1.f);
		glScalef(PILLAR_CIRCLE_RADIUS, PILLAR_CIRCLE_RADIUS, PILLAR_CIRCLE_RADIUS);
		glutSolidOctahedron();
		glPopMatrix();
	}
	else {
		glColor4f(0.25f, 0.125f, 0.f, 1.f);
		glutSolidCylinder(PILLAR_CIRCLE_RADIUS, y, 30, 1);
	}

	
	if (select_front_view)	glColor4f(1.f, 0.f, 0.f, 1.f);
	else glColor4f(0.f, 0.f, 0.f, 1.f);

	if (rock) {
		glPushMatrix();
		glScalef(PILLAR_CIRCLE_RADIUS+1, PILLAR_CIRCLE_RADIUS+1, PILLAR_CIRCLE_RADIUS+1);
		glutWireOctahedron();
		glPopMatrix();
	}
	else glutWireCylinder(PILLAR_CIRCLE_RADIUS + 1, y, 30, 1);
	

	if (top_view) {
		if(top_view_draw_collision || click_mouse_collision)		glColor4f(1.f, 0.f, 0.f, 1.f);
		else glColor4f(1.f, 1.f, 0.f, 1.f);
		glutWireCylinder(PILLAR_CIRCLE_RADIUS + access_size, 2, 30, 1);
	}
	glPopMatrix();
}

void Pillar::Update(){
	
}




