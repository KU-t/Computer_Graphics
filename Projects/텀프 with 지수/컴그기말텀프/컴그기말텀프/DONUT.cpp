#include "DONUT.h"
#include "main.h"


DONUT::DONUT()
{
}
DONUT::DONUT(float in_x, float in_y, float in_z) :x(in_x), y(in_y), z(in_z) {

}

DONUT::~DONUT()
{
}

void DONUT::Draw() {

	// 신호등 쨈도넛
	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(0, 20, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	if (type == 0)
		glColor4f(1.f, 0.f, 0.f, 1.f);
	if (type == 1)
		glColor4f(0.f, 1.f, 0.f, 1.f);
	if (type == 2)
		glColor4f(1.f, 1.f, 0.f, 1.f);
	glutSolidSphere(40.f, 10, 10); // 가운데거
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(0, 20, 0);
	glRotatef(rotation, 0.f, 1.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(0.5, 0.1, 0.f, 1.f);
	glutSolidTorus(30, 40, 10, 20); // 링
	glPopMatrix();

}

void DONUT::Draw_Collision_rect() {
	glPushMatrix();
	glTranslatef(x, y + 65, z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(0.5f, 0.5f, 0.5f, 1.f);
	glutWireSphere(collision_size, 10, 10);
	glPopMatrix();
}

void DONUT::Update() {
	rotation++;
	x++;
}

