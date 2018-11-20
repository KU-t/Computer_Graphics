#include "ICE.h"
#include "main.h"


ICE::ICE()
{
}
ICE::ICE(float in_x, float in_y, float in_z) :x(in_x), y(in_y), z(in_z) {

}


ICE::~ICE()
{
}
void ICE::Draw() {
	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(1.f, 0.8f, 0.5f, 1.f);
	glutSolidCone(60, 100, 10, 10); // ÄÜ

	//glColor4f(0.f, 0.f, 0.f, 1.f);
	//glutWireCone(70, 101, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(0, 20, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(r, g, b, 1.f);
	glutSolidSphere(40.f, 10, 10); // ±¸
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(0, 20, 0);
	glRotatef(rotation, 0.f, 1.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(0.8f, 0.8f, 0.8f, 1.f);
	glutSolidTorus(20, 40, 20, 20);
	glPopMatrix();
}

void ICE::Draw_Collision_rect() {
	glPushMatrix();
	glTranslatef(x, y + 40, z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(0.5f, 0.5f, 0.5f, 1.f);
	glutWireSphere(collision_size, 10, 10);
	glPopMatrix();
}

void ICE::Update() {
	x++;
	rotation++;
	r = 1.0f - (rand() % 5) * 0.1;
	g = 1.0f - (rand() % 5) * 0.1;
	b = 1.0f - (rand() % 5) * 0.1;
}
