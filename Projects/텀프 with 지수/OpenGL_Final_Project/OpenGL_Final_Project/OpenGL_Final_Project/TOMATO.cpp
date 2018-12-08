#include "TOMATO.h"

void TOMATO::Draw() {
	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Object_specref0[] = { 0.f, 0.f, 0.f, 1.f };

	glPushMatrix();
	float lengh = 40.f;
	glTranslatef(x, y, z);
	//glutWireSphere(TOMA_COLL, 10, 10);
	glRotatef(90.f, 1.f, 1.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glScalef(0.5f, 0.5f, 0.5f);

	GLfloat Object3[] = { 1.f, 0.01f, 0.01f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	for (float deg = 0.f; deg < 360.f; deg += 90.f) {
		glPushMatrix();
		glScalef(0.9, 1.3, 1.f);
		float x = lengh * cos(deg(deg));
		float z = lengh * sin(deg(deg));
		glTranslatef(x, 0, z);
		glutSolidSphere(110, 100, 100);
		//glutWireSphere(51, 10, 10);
		glPopMatrix();
	}

	GLfloat Object1[] = { 0.01f, 1.f, 0.01f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref0);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	for (float i = 0.f; i < 180.f; i += 30.f) {
		glPushMatrix();
		glTranslatef(0, 100, 0);
		glRotatef(i, 0.f, 1.f, 0.f);
		glScalef(0.2f, 1.f, 1.f);
		glutSolidSphere(size + 5, 100, 100);
		glPopMatrix();
	}

	glPopMatrix();



}

void TOMATO::Update(float Huddle_speed) {
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

TOMATO::TOMATO() {
}

TOMATO::TOMATO(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max) :x(in_x), y(in_y), z(in_z), moving(in_moving), data_min(in_data_min), data_max(in_data_max) {
	data = 0;

}


TOMATO::~TOMATO()
{
}
