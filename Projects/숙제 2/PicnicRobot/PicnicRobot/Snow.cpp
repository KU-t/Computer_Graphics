#include "Snow.h"



Snow::Snow(){
	i = rand() % (WINDOW_SIZE_X / 10);
	j = rand() % (WINDOW_SIZE_Z / 10);
	y = rand() % 1600;
}

void Snow::Draw(int weather) {
	if (weather != 0) {

		glPushMatrix();
		glTranslatef(-WINDOW_SIZE_X + (20 * i), y - WINDOW_SIZE_Y / 2, -WINDOW_SIZE_Z + (20 * j));

		if (weather == 1) {
			glColor4f(1.f, 1.f, 1.f, 1.f);
			glutSolidSphere(10.f, 20, 20);
		}

		if (weather == 2) {
			glColor4f(0.f, 0.f, 1.f, 1.f);
			glPushMatrix();
			glRotatef(-90.f, 1.f, 0.f, 0.f);
			glutSolidCone(5.f, 10.f, 100, 100);
			glPopMatrix();
			glTranslatef(0.f, -3.f, 0.f);
			glutSolidSphere(5, 100, 100);
		}

		glPopMatrix();
	}
}


Snow::~Snow(){

}
