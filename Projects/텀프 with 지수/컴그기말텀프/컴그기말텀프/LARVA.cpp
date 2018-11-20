#include "LARVA.h"
#include "main.h"

LARVA::LARVA()
{
}
LARVA::LARVA(float in_x, float in_y, float in_z) :x(in_x), y(in_y), z(in_z) {

}

LARVA::~LARVA()
{
}

void LARVA::Draw() {

	GLfloat Object0[] = { 0.1f, 0.5f, 0.1f, 1.f };
	GLfloat Object_specref0[] = { 0.3f, 0.5f, 0.3f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref0);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	// 赣府
	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(0, 100, 0);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(0.7f, 1.0f, 0.f, 1.f);
	glutSolidSphere(40.f, 10, 10); // 备
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(50, 90, 0);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(0.1f, 0.5f, 0.f, 1.f);
	glutSolidSphere(30.f, 10, 10); // 备
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(90, 80, 0);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(0.1f, 0.5f, 0.f, 1.f);
	glutSolidSphere(25.f, 10, 10); // 备
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(120, 70, 0);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(0.1f, 0.5f, 0.f, 1.f);
	glutSolidSphere(20.f, 10, 10); // 备
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 50, z);
	glTranslatef(140, 70, 0);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(0.1f, 0.5f, 0.f, 1.f);
	glutSolidSphere(15.f, 10, 10); // 备
	glPopMatrix();
}

void LARVA::Draw_Collision_rect() {
	glPushMatrix();
	glTranslatef(x + 50, y + 140, z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(0.5f, 0.5f, 0.5f, 1.f);
	glutWireSphere(collision_size, 10, 10);
	glPopMatrix();
}

void LARVA::Update() {
	rotation++;
	x--;
}

