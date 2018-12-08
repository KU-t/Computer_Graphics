#include "BALL.h"



BALL::BALL(){
}

BALL::BALL(float in_x, float in_y, bool in_type):x(in_x), y(in_y),type(in_type){
	z = 0.f;
	vel_y = BALL_INIT_Y_VEL;
	vel_z = BALL_INIT_Z_VEL;
}


BALL::~BALL()
{
}

void BALL::Draw(){
	GLfloat Object[] = { 0.1f, 0.1f, 0.7f, 1.f };
	if (type) {
		Object[0] = 0.7f;
		Object[2] = 0.1f;
	}

	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(rad, 1.f, 0.f, 0.f);
	glutSolidSphere(BALL_SIZE, 100, 100);
	glPopMatrix();
}

void BALL::Update(){
	vel_y += GRAVITY;
	y = y + vel_y;
	if (y < -WINDOW_SIZE_Z) {
		y = -WINDOW_SIZE_Z + BALL_SIZE;
		vel_y = -0.75 * vel_y;
	}

	z = z - vel_z;
}
