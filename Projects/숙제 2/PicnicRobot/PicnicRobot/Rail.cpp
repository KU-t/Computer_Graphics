#include "Rail.h"
#include "ingame.h"


Rail::Rail(){
	for (int i = 0; i < MAX_TUNNEL; i++) {
		tunnel_x[i] = 0.f;
		tunnel_y[i] = -300.f;
		tunnel_z[i] = 0.f;
	}

}

Rail::~Rail()
{
}

void Rail::Draw(){
	glPushMatrix();
	glColor4f(0.25f, 0.125f, 0.f, 1.f);
	glutSolidCube(5);

	glPushMatrix();
	glTranslatef(-5.f, 0.f, 0.f);
	glutSolidSphere(5, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.f, 0.f, 0.f);
	glutSolidSphere(5, 10, 10);
	glPopMatrix();

	glPopMatrix();
}
