#include "FLOWER.h"
#include "main.h"

FLOWER::FLOWER()
{
}
FLOWER::FLOWER(float in_x, float in_y, float in_z) :x(in_x), y(in_y), z(in_z) {

}

FLOWER::~FLOWER()
{
}

void FLOWER::Draw() {
	glPushMatrix();
	glTranslatef(x, y + 20, z);
	glTranslatef(0, 60, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(1.f, 0.7f, 0.5f, 1.f);
	GLfloat Object1[] = { 0.1f, 0.1f, 0.f, 1.f };
	GLfloat Object_specref1[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref1);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glutSolidSphere(35.f, 10, 10); // 备1
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 20, z);
	glTranslatef(40, 25, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(1.f, 0.7f, 0.5f, 1.f);
	GLfloat Object2[] = { 0.1f, 0.5f, 0.5f, 1.f };
	GLfloat Object_specref2[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref2);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glutSolidSphere(35.f, 10, 10); // 备2
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 20, z);
	glTranslatef(-40, 25, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(1.f, 0.7f, 0.5f, 1.f);
	GLfloat Object3[] = { 0.5f, 0.1f, 0.5f, 1.f };
	GLfloat Object_specref3[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref3);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glutSolidSphere(35.f, 10, 10); // 备3
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 20, z);
	glTranslatef(-30, -15, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(1.f, 0.7f, 0.5f, 1.f);
	GLfloat Object4[] = { 0.5f, 0.5f, 0.5f, 1.f };
	GLfloat Object_specref4[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref4);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glutSolidSphere(35.f, 10, 10); // 备4
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x, y + 20, z);
	glTranslatef(30, -15, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(1.f, 0.7f, 0.5f, 1.f);
	GLfloat Object5[] = { 0.5f, 0.1f, 0.f, 1.f };
	GLfloat Object_specref5[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object5);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref5);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glutSolidSphere(35.f, 10, 10); // 备5
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x, y + 20, z);
	glTranslatef(0, 20, 0);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rotation, 0.f, 0.f, 1.f);
	glColor4f(r, g, b, 1.f);
	GLfloat Object6[] = { 0.1f, 0.5f, 0.f, 1.f };
	GLfloat Object_specref6[] = { 0.5f, 0.5f, 0.f, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object6);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref6);
	glMateriali(GL_FRONT, GL_SHININESS, 64);
	glutSolidSphere(45.f, 10, 10); // 备
	glPopMatrix();
}

void FLOWER::Draw_Collision_rect() {
	glPushMatrix();
	glTranslatef(x, y + 40, z);
	glRotatef(90.f, 1.f, 0.f, 0.f);
	glRotatef(rad, 0.f, 1.f, 0.f);
	glColor4f(0.5f, 0.5f, 0.5f, 1.f);
	glutWireSphere(collision_size, 10, 10);
	glPopMatrix();
}

void FLOWER::Update() {
	rotation++;
	x--;
	r = 1.0f - (rand() % 5) * 0.1;
	g = 1.0f - (rand() % 5) * 0.1;
	b = 1.0f - (rand() % 5) * 0.1;
}
