#include "Rail.h"
#include "ingame.h"

Rail::Rail()
{
}

Rail::~Rail()
{
}

void Rail::Draw(int num_pillar, int t){
	glPushMatrix();
	glColor4f(0.25f, 0.125f, 0.f, 1.f);
	glutSolidCube(10);

	glPushMatrix();
	glTranslatef(-10.f, 0.f, 0.f);
	glutSolidSphere(10, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10.f, 0.f, 0.f);
	glutSolidSphere(10, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.f, 20.f, 0.f);
	glScalef(1.f, 1.f, 0.3f);
	glColor4f(1.f, 0.f, 0.f, 1.f);
	glutSolidCube(20);
	glPopMatrix();

	glPopMatrix();
}
