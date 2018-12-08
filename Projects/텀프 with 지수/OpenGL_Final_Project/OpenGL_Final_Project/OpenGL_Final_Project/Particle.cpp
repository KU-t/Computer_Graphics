#include "Particle.h"


Point::Point() {

}

void Point::Init(float in_x, float in_y, float in_z, float in_vel_x, float in_vel_y, float in_vel_z) {
	x = in_x;
	y = in_y;
	z = in_z;
	vel_x = in_vel_x;
	vel_y = in_vel_y;
	vel_z = in_vel_z;
}

void Point::Update() {
	if (x < -WINDOW_SIZE_X) {
		x = -WINDOW_SIZE_X;
		vel_x = 0;
		vel_y = 0;
	}
	else if (WINDOW_SIZE_X < x) {
		x = WINDOW_SIZE_X;
		vel_x = 0;
		vel_y = 0;
	}
	else x = x + vel_x;

	if (y < -WINDOW_SIZE_Y) {
		y = -WINDOW_SIZE_Y;
		vel_x = 0;
		vel_y = 0;
	}
	else if (WINDOW_SIZE_Y < y) {
		y = WINDOW_SIZE_Y;
		vel_x = 0;
		vel_y = 0;
	}
	else {
		//vel_y += GRAVITY / 100;
		y = y + vel_y;
	}

	if (!(vel_x == 0 || vel_y == 0))
		z = z + vel_z;
}

PARTICLE::PARTICLE() {

}

PARTICLE::PARTICLE(float x, float y, float z, float r, float g, float b) : x(x), y(y), z(z), r(r), g(g), b(b) {
	speed = HUDDLE_MOVE_SPEED_1;
	for (int i = 0; i < MAX_PARTICLE_PART; i++) {
		p[i].Init(x, y, z, rand() % MAX_INIT_VEL - HALF_MAX_INIT_VEL, rand() % MAX_INIT_VEL - HALF_MAX_INIT_VEL, (rand() % MAX_INIT_VEL) / 3);
	}
}

PARTICLE::~PARTICLE() {

}

void PARTICLE::Draw() {
	GLfloat Object_specref[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat Object_specref0[] = { 0.f, 0.f, 0.f, 1.f };
	GLfloat Object3[] = { r, g, b, 1.f };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Object3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Object_specref);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glPushMatrix();
	glTranslatef(x, y, z);
	for (int i = 0; i < MAX_PARTICLE_PART; i++) {
		glPushMatrix();
		glTranslatef(p[i].x, p[i].y, p[i].z);
		glutSolidSphere(PARTICLE_SIZE, 3, 3);
		glPopMatrix();
	}
	glPopMatrix();
}

void PARTICLE::Update() {
	z += speed;
	for (int i = 0; i < MAX_PARTICLE_PART; i++) {
		p[i].Update();
	}
}