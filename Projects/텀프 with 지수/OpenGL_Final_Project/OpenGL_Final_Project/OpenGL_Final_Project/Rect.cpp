#include "Rect.h"

void OBSRECT::Draw() {

	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Object_specref0[] = { 0.f, 0.f, 0.f, 1.f };

	glPushMatrix();
	glTranslatef(x, y, z);
	
	
	GLfloat Object3[] = { 0.3f, 0.3f, 0.7f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref0);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glScalef(2*WINDOW_SIZE_X, RECT_SIZE, 1);
	glutSolidCube(1);

	glPopMatrix();
}

void OBSRECT::Update(float Huddle_speed) {
	switch (moving) {
	case 2:
		if (data == 0) {
			if (y < data_max)   y += HUDDLE_WAVE_MOVE;
			else data = 1;
		}

		if (data == 1) {
			if (y > data_min)   y -= HUDDLE_WAVE_MOVE;
			else data = 0;
		}
		break;
	}
	z += Huddle_speed;
}

OBSRECT::OBSRECT() {

}

OBSRECT::OBSRECT(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max) :x(in_x), y(in_y), z(in_z), moving(in_moving), data_min(in_data_min), data_max(in_data_max) {
	data = 0;
}


OBSRECT::~OBSRECT()
{
}

