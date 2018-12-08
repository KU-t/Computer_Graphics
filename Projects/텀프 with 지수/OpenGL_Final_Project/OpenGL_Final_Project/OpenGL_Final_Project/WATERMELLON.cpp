#include "WATERMELLON.h"

void WATERMELLON::Draw() {
	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };

	glPushMatrix();

	glTranslatef(x, y, z);
	//glutWireSphere(WATERM_COLL, 10, 10);
	glRotatef(90.f, 1.f, 1.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);

	glPushMatrix();
	GLfloat Object0[] = { 0.01f, 1.0f, 0.01f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glutSolidSphere(size, 100, 100);
	glPopMatrix();

	GLfloat Object1[] = { 0.f, 0.f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	for (float i = 0.f; i < 180.f; i += 30.f) {
		glPushMatrix();
		glRotatef(i, 0.f, 1.f, 0.f);
		glScalef(0.2f, 1.f, 1.f);
		glutSolidSphere(size + 1, 100, 100);
		glPopMatrix();
	}
	glPopMatrix();

}

void WATERMELLON::Update(float Huddle_speed) {
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

WATERMELLON::WATERMELLON() {
}

WATERMELLON::WATERMELLON(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max) :x(in_x), y(in_y), z(in_z), moving(in_moving), data_min(in_data_min), data_max(in_data_max) {
	data = 0;

}



WATERMELLON::~WATERMELLON()
{
}
