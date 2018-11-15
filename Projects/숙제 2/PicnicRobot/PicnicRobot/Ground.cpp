#include "Ground.h"
#include "Ingame.h"


Ground::Ground(){
	x = 0;
	y = 0;
	z = 0.f;
}

void Ground::Draw(){
	glPushMatrix();
	glTranslatef(0.f, -WINDOW_SIZE_X  * 3 / 4, 0.f);
	glPushMatrix();
	glScalef(2 * WINDOW_SIZE_X, WINDOW_SIZE_X / 2, 2 * WINDOW_SIZE_Y);
	glColor4f(0.5f, 0.25f, 0.f, 1.f);
	glutSolidCube(1.f);
	glPopMatrix();
	glScalef(2 * WINDOW_SIZE_X + 1, WINDOW_SIZE_X / 2 + 1, 2 * WINDOW_SIZE_Y + 1);
	glColor4f(0.f, 0.f, 0.f, 1.f);
	glutWireCube(1.f);
	glPopMatrix();
}

