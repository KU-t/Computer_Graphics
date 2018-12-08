#include "PUMPKIN.h"
#include "main.h"


PUMPKIN::PUMPKIN()
{
}
PUMPKIN::PUMPKIN(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max) :x(in_x), y(in_y), z(in_z), moving(in_moving), data_min(in_data_min), data_max(in_data_max) {
	data = 0;

}


PUMPKIN::~PUMPKIN()
{
}

void PUMPKIN::Draw() {
	float lengh = 40.f;
	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rad, 0.f, 1.f, 0.f);
	//glutWireSphere(PUMP_COLL, 10, 10);

	GLfloat Object3[] = { 1.f, 0.5f, 0.01f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	for (float deg = 0.f; deg < 360.f; deg += 60.f) {
		glPushMatrix();
		float mx = lengh * cos(deg(deg));
		float mz = lengh * sin(deg(deg));
		glTranslatef(mx, 0, mz);
		glutSolidSphere(50, 100, 100);
		glPopMatrix();
	}
	GLfloat Object0[] = { 0.01f, 1.f, 0.01f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(0, 100, 0);

	glPushMatrix();
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glutSolidCone(10, 80, 10, 10);
	glPopMatrix();

	glPopMatrix();
	glPopMatrix();

}

void PUMPKIN::Update(float Huddle_speed) {
	//rotation++;
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

