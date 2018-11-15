#include "Pillar.h"
#include "ingame.h"


Pillar::Pillar(){

}

Pillar::Pillar(float in_x, float in_z) : x(in_x), z(in_z){
	y = PILLAR_HEIGHT;
	circle_rad = PILLAR_CIRCLE_RADIUS;
}

void Pillar::Draw(){
	glPushMatrix();
	glTranslatef(x, y - (WINDOW_SIZE_Y / 2), z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glColor4f(0.25f, 0.125f, 0.f, 1.f);
	glutSolidCylinder(circle_rad, y, 30, 1);	
	if (select_mouse)	glColor4f(1.f, 0.f, 0.f, 1.f);
	else glColor4f(0.f, 0.f, 0.f, 1.f);
	glutWireCylinder(circle_rad + 1, y, 30, 1);
	if (top_view) {
		glColor4f(1.f, 1.f, 0.f, 1.f);
		glutWireCylinder(circle_rad + PILLAR_BUILD_ACCESS, 2, 30, 1);
	}
	glPopMatrix();
}

void Pillar::Update(){
	
}




