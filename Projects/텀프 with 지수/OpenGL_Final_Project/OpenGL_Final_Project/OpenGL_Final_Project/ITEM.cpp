#include "ITEM.h"



void ITEM::Draw(){

	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Object[] = { 0.5f, 0.5f, 0.01f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rad, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glRotatef(rad, 0.f, 0.f, 1.f);
	//glutWireSphere(ITEM_COLL, 10, 10);
	
	glPushMatrix();
	for (int i = 0; i <= 360; i += 45) {
		for (int j = 0; j <= 360; j += 45) {
			for (int k = 0; k <= 360; k += 45) {
				glPushMatrix();
				glRotatef(i, 1, 0, 0);
				glRotatef(j, 0, 1, 0);
				glRotatef(k, 0, 0, 1);
				glutSolidCone(50, 100, 5, 5);
				glPopMatrix();
			}
		}
	}
	glPopMatrix();

	glPopMatrix();
}

void ITEM::Update(float Huddle_speed){
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

ITEM::ITEM(){

}

ITEM::ITEM(float in_x, float in_y, float in_z, int in_moving, float in_data_min, float in_data_max) :x(in_x), y(in_y), z(in_z), moving(in_moving), data_min(in_data_min), data_max(in_data_max){
	data = 0;
}


ITEM::~ITEM(){

}

