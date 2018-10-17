#include <gl/freeglut.h>
#include <math.h>

enum MOVE{p1_p2,p2_p3,p3_p1};

#define W_x 800
#define W_y 600
#define W_z 600
#define ROOT3 1.414
#define CIRCLE_X(x) cos(x * 3.141592 / 180) 
#define CIRCLE_Y(y) sin(y * 3.141592 / 180)
#define BIG_SIZE 500.f
#define SMALL_SIZE 100.f
#define count_size 100
class point {
public:
	float x, y, z;
	point(){
		x = 0.f;
		y = 500.f;
		z = 0.f;
	}
};

class Point {
public:
	float x, y, z;
	float angle, scale;
	point p[3];
	MOVE m;
	int count;

	Point(){
		angle = 0;
		scale = 0;
		m = p1_p2;
		count = 0;
	}

	void move_point(point p1, point p2) {
		float t = count / 100.f;
		x = (1 - t)*p1.x + t * p2.x;
		y = (1 - t)*p1.y + t * p2.y;
		z = (1 - t)*p1.z + t * p2.z;
	}

	void move(){
		switch (m){
		case p1_p2:
			move_point(p[0], p[1]);
			if (count >= count_size ) {
				m = p2_p3;
			}
			break;

		case p2_p3:
			move_point(p[1], p[2]);
			if (count == count_size) {
				m = p3_p1;
			}
			break;

		case p3_p1:
			move_point(p[2], p[0]);
			if (count == count_size) {
				m = p1_p2;
			}
			break;
		}
		count = (count + 1) % (count_size + 1);
	}

};

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Timer(int value);
void Keyboard(unsigned char key, int x, int y);
void Draw_TRIANGLE(int model, float x, float y, float z, float size, float angle, float spin_x, float spin_y, float spin_z);

float angle = 0.f;
float scale = 1.f;
	
Point p1, p2;

void main(int argc, char **argv) {
	p1.p[1].x = BIG_SIZE * CIRCLE_X(210);
	p1.p[1].y = BIG_SIZE * CIRCLE_Y(210);
	p1.p[2].x = BIG_SIZE * CIRCLE_X(330);
	p1.p[2].y = BIG_SIZE * CIRCLE_Y(330);

	p2.p[1].y = BIG_SIZE * CIRCLE_Y(210);
	p2.p[1].z = BIG_SIZE * CIRCLE_X(210);
	p2.p[2].y = BIG_SIZE * CIRCLE_Y(330);
	p2.p[2].z = BIG_SIZE * CIRCLE_X(330);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_x, W_y);
	glutCreateWindow("3Àå ½Ç½À 13");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutTimerFunc(50, Timer, 1);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINES);
	glVertex3f(-W_x,0,0);
	glVertex3f(W_x,0,0);
	glVertex3f(0,-W_y,0);
	glVertex3f(0,W_y,0);
	glVertex3f(0,0,-W_z);
	glVertex3f(0,0,W_z);
	glEnd();

	glPushMatrix();
	glRotatef(angle, 0.f, 1.f, 0.f);
	glScalef(scale, scale, scale);

	Draw_TRIANGLE(1, 0, 0, 0, BIG_SIZE, 0, 0, 0, 0);
	Draw_TRIANGLE(3, 0, 0, 0, BIG_SIZE, -90, 1, 0, 0);

		glPushMatrix();

			glPushMatrix();
				glTranslatef(p2.x, p2.y, p2.z);
				glRotatef(90 + p2.angle, 0.0f, 1.f, 0.f);
				glScalef(1 / scale, 1 / scale, 1 / scale);
				Draw_TRIANGLE(1, 0, 0, 0, SMALL_SIZE, 0, 0, 0, 0);
			
			glPopMatrix();
			glTranslatef(p1.x, p1.y, p1.z);
			glRotatef(p1.angle, 0.0f, 1.f, 0.f);
			glScalef(1 / scale, 1 / scale, 1 / scale);
			Draw_TRIANGLE(1, 0, 0, 0, SMALL_SIZE, 0, 0, 0, 0);
		
		glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(-W_x, W_x, -W_y, W_y, -W_z, W_z);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	glutPostRedisplay();
}

void Motion(int x, int y) {

	glutPostRedisplay();
}

void Timer(int value) {
	p1.angle+=2;
	p2.angle+=2;
	p1.move();
	p2.move();
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '[': case'{':
		angle -= 2.f;
		break;

	case ']': case'}':
		angle += 2.f;
		break;

	case '-': case'_':
		if (scale > 0.7f)
			scale -= 0.1f;
		break;

	case '=': case'+':
		if (scale < 1.3f)
			scale += 0.1f;
		break;
	}
}

void Draw_TRIANGLE(int model,float x, float y, float z, float size, float angle, float spin_x, float spin_y, float spin_z){
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, spin_x, spin_y, spin_z);
	glBegin(GL_LINE_LOOP);
	switch (model) {
	case 1:
		glVertex3f(size * CIRCLE_X(90), size * CIRCLE_Y(90), 0.f);
		glVertex3f(size * CIRCLE_X(210), size * CIRCLE_Y(210), 0.f);
		glVertex3f(size * CIRCLE_X(330), size * CIRCLE_Y(330), 0.f);
		break;

	case 2:
		glVertex3f(size * CIRCLE_X(90), 0.f, size * CIRCLE_Y(90));
		glVertex3f(size * CIRCLE_X(210), 0.f, size * CIRCLE_Y(210));
		glVertex3f(size * CIRCLE_X(330), 0.f, size * CIRCLE_Y(330));
		break;

	case 3:
		glVertex3f(0.f, size * CIRCLE_X(90), size * CIRCLE_Y(90));
		glVertex3f(0.f, size * CIRCLE_X(210), size * CIRCLE_Y(210));
		glVertex3f(0.f, size * CIRCLE_X(330), size * CIRCLE_Y(330));
		break;
	}
	glEnd();

	glPopMatrix();
	glutPostRedisplay();
}

