#include "Ground.h"
#include "Ingame.h"


Ground::Ground(){
	x = 0;
	y = 0;
	z = 0.f;
}

void Ground::Draw(){
	glPushMatrix();
	glTranslatef(0.f, -WIDNOW_SIZE_X  * 3 / 4, 0.f);
	glScalef(2 * WIDNOW_SIZE_X, WIDNOW_SIZE_X / 2, 2 * WIDNOW_SIZE_Y);
	glColor3f(1.f, 1.f, 0.f);
	glColor4f(0.5f, 0.25f, 0.f, 1.f);
	glutSolidCube(1.f);
	glPopMatrix();
}

