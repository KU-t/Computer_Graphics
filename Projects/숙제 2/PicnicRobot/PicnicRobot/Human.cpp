#include "Human.h"



Human::Human(){
	x = 0;
	y = - 300;
	z = 0;
	choice = false;
}


Human::~Human()
{
}

void Human::Update(){

	if (hand_foot) {
		if (rad_hand_foot >= 30.f)
			hand_foot = false;
		rad_hand_foot+=10.f;
	}
	else {
		if (rad_hand_foot <= -30.f)
			hand_foot = true;
		rad_hand_foot-=10.f;
	}
	
	
}

void Human::Draw(){

	glPushMatrix(); // »ç¶÷
	glTranslatef(x, y, z);
	//glutWireSphere(HUMAN_SIZE, 10, 10);
	
	if (choice) {
		glPushMatrix();
		glTranslatef(0.f, 100.f, 0.f);
		glRotatef(90.f, 1.f, 0.f, 0.f);
		glColor4f(1.f, 1.f, 0.f, 1.f);
		glutSolidCone(10, 70, 100, 100);
		glPopMatrix();
	}

	glRotatef(rad_move, 0.f, 1.f, 0.f);
					//glTranslatef(-40.f, 115.f, 0.f);

					//glPushMatrix(); // ÆÈ
					//glColor4f(1.f, 0.8f, 0.6f, 1.f);
					//glRotatef(-90.f, 0.f, 0.f, 1.f);
					//glRotatef(-180.f, 0.f, 1.f, 0.f);
					//for (float i = -30.f; i < 60.f; i += 60.f) {
					//	glPushMatrix(); // 1
					//	glTranslatef(-40.f, 15.f, i);
					//	glRotatef(0.f, 0.1, 0.1, 0.1);
					//	glScalef(10.f, 40.f, 10.f);
					//	glutSolidCube(1);
					//	glPopMatrix(); // 1
					//}
					//glPopMatrix(); // ÆÈ
	
	glPushMatrix(); // ÆÈ
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	for (float i = -1.f; i < 2.f; i += 2.f) {
		glPushMatrix();
		glTranslatef(0.f, -30.f, i * 30.f);
		glRotatef(-i * rad_hand_foot, 0.f, 0.f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -20.f, 0.f);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
	}
	glPopMatrix(); // ÆÈ

	glPushMatrix(); // 1
	glTranslatef(0.f, -20.f, 0.f);
	glColor4f(0.f, 0.f, 0.f, 0.f);
	glutSolidSphere(20.f, 10.f, 10.f);
	glTranslatef(10.f, -5.f, 0.f);
	glColor4f(1.f, 0.8f, 0.6f, 1.f);
	glutSolidSphere(15.f, 10.f, 10.f);
	for (float i = -5.f; i < 15.f; i += 10.f) {
		glPushMatrix();
		glTranslatef(13.f, -2.f, i);
		glColor4f(0.f, 0.f, 0.f, 1.f);
		glutSolidSphere(2.f, 10.f, 10.f);
		glPopMatrix();
	}
	glPopMatrix(); // 1

	glPushMatrix();
	glTranslatef(0.f, -50.f, 0.f);
	glScalef(15.f, 40.f, 50.f);
	glColor4f(0.7f, 0.f, 0.f, 1.f);
	glutSolidCube(1);
	glPopMatrix();


	glPushMatrix(); // ´Ù¸®
	for (float i = -1.f; i < 2.f; i += 2.f) {
		glColor4f(0.f, 0.f, 0.7f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -60.f, i * 15.f);
		glRotatef(i * rad_hand_foot, 0.f, 0.f, 1.f);
		glPushMatrix();
		glTranslatef(0.f, -20.f, 0.f);
		glScalef(10.f, 40.f, 10.f);
		glutSolidCube(1);
		glPopMatrix();
		glPopMatrix();
	}
	glPopMatrix(); // ´Ù¸®

	glPopMatrix(); // »ç¶÷

}