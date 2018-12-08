#include "GRAPE.h"

void GRAPE::Draw() {

	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };

	glPushMatrix();

	glTranslatef(x, y, z);
	glRotatef(rad, 0.f, 1.f, 0.f);
	//glutWireSphere(GRAPE_COLL, 10, 10);

	glPushMatrix();
	glTranslatef(0.f, 30.f, 0.f);
	GLfloat Object0[] = { 0.5f, 0.1f, 0.8f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glutSolidSphere(size, 100, 100); // 가운데
	glTranslatef(-30, 0, 0); // 맨위왼
	glutSolidSphere(size, 100, 100);

	glTranslatef(60, 0, 0);
	glutSolidSphere(size, 100, 100);

	glTranslatef(-20, -45, 0);
	glutSolidSphere(size, 100, 100);

	glTranslatef(-30, 0, 0);
	glutSolidSphere(size, 100, 100);

	glTranslatef(15, -45, 0);
	glutSolidSphere(size, 100, 100);



	glPushMatrix();
	glTranslatef(0, 160, 0);
	GLfloat Object1[] = { 0.01f, 1.f, 0.01f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glutSolidCone(10, 80, 10, 10);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void GRAPE::Update(float Huddle_speed) {
	switch (moving) {
	case 1:
		if (data == 0) {
			if (x < data_max)   x += HUDDLE_WAVE_MOVE;
			else data = 1;
		}

		if (data == 1) {
			if (x > data_min)   x -= HUDDLE_WAVE_MOVE;
			else data = 0;
		}
		break;

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
	rad += 10.f;

	z += Huddle_speed;
}

GRAPE::GRAPE() {
}

GRAPE::GRAPE(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max) :x(in_x), y(in_y), z(in_z), moving(in_moving), data_min(in_data_min), data_max(in_data_max) {
	data = 0;

}



GRAPE::~GRAPE()
{
}