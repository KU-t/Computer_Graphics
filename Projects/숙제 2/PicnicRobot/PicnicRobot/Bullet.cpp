#include "Bullet.h"



Bullet::Bullet()
{
}

Bullet::Bullet(float in_x, float in_y, float in_z, float in_rad) : x(in_x), y(in_y),z(in_z),dir(in_rad){

}


Bullet::~Bullet()
{
}

void Bullet::Draw(){
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor4f(1.f, 1.f, 0.3f, 1.f);
	glutSolidSphere(BULLET_SIZE, 100, 100);
	glPopMatrix();
}

void Bullet::Update(){
	if (dir == 0)	x += BULLET_SPEED;

	else if (dir == 90)	z -= BULLET_SPEED;
	
	else if (dir == 180)	x -= BULLET_SPEED;
	
	else if (dir == 270)	z += BULLET_SPEED;
	
}
