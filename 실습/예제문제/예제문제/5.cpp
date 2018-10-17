#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUMBER 10

enum { RT, RB, LT, LB };
enum {Rec, Tri};
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void main(int argc, char ** argv);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);

int ani[NUMBER] = {};
int NUM = 0;
int mx[NUMBER] = { 0 }, my[NUMBER] = { 0 }, size[NUMBER] = { 0 };
int way[NUMBER];
bool move[NUMBER];
int speed;
bool Shape[NUMBER];

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));

	for (int i = 0; i < NUMBER; i++) {
		ani[NUMBER] = 1;
		move[i] = false;
		Shape[i] = Rec;
	}
	speed = 0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3Àå ½Ç½À 5");
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(50, Timer, 1);
	glutMainLoop();
}


GLvoid drawScene() {
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < NUMBER; i++) {
		if (mx[i] != 0 && my[i] != 0) {
			glColor4f((rand() % 10) % 10 * 0.1, (rand() % 10) % 10 * 0.1, (rand() % 10) % 10 * 0.1, 0);
			if (Shape[i] == Rec)
				glRectf(mx[i], my[i], mx[i] + size[i], my[i] + size[i]);
			else if (Shape[i] == Tri) {
				glBegin(GL_POLYGON);
				glVertex2i(mx[i], my[i] + size[i] / 2);
				glVertex2i(mx[i] + size[i] / 2, my[i] - size[i] / 2);
				glVertex2i(mx[i] - size[i] / 2, my[i] - size[i] / 2);
				glEnd();
			}
		}
	}
	glFlush();
}


GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, 800.0, 0, 600.0, -1.0, 1.0);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mx[NUM] = x;
		my[NUM] = 600 - y;
		size[NUM] = rand() % 20 + 10;
		way[NUM] = rand() % 4;
		move[NUM] = true;
		++NUM;

		if (NUM > NUMBER - 1) {
			NUM = 0;
		}

		glutPostRedisplay();
	}
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '+':	case '=':
		if (speed <= 10)	speed++;
		break;

	case '-':	case '_':
		if (0 <= speed)	speed--;
		break;

	case 'c':	case 'C':
		if (Shape[NUM] == Rec)	Shape[NUM] = Tri;
		else if (Shape[NUM] == Tri)	Shape[NUM] = Rec;
		break;
	}
	glutPostRedisplay();
}

void Timer(int value) {
	for (int i = 0; i < NUMBER; i++) {
		size[i] += ani[i];
		if (size[i] >= 50)	ani[i] = -1;
		else if (size[i] <= 30)	ani[i] = 1;
	
		if (move[i] == true) {
			switch (way[i]) {
			case RT:
				if (mx[i] + size[i] >= 800)	way[i] = LT;
				else if (my[i] + size[i] >= 600) way[i] = RB;
				mx[i] += speed;
				my[i] += speed;
				break;

			case RB:
				if (mx[i] + size[i] >= 800) way[i] = LB;
				else if (my[i] <= 0) way[i] = RT;
				mx[i] += speed;
				my[i] -= speed;
				break;

			case LT:
				if (mx[i] <= 0) way[i] = RT;
				else if (my[i] + size[i] >= 600) way[i] = LB;
				mx[i] -= speed;
				my[i] += speed;
				break;

			case LB:
				if (mx[i] <= 0) way[i] = RB;
				else if (my[i] <= 0) way[i] = LT;
				mx[i] -= speed;
				my[i] -= speed;
				break;
			}
		}
	
	}
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}