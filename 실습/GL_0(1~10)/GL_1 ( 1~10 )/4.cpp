#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUMBER 10


GLvoid drawScene(GLvoid);

GLvoid Reshape(int w, int h);
void main(int argc, char ** argv);
void Mouse(int button, int state, int x, int y);
void Timer(int value);

int ani[NUMBER] = {  };
int NUM = 0;
int mx[NUMBER] = { 0 }, my[NUMBER] = { 0 }, size[NUMBER] = { 0 };

void main(int argc, char **argv) {
	srand((unsigned)time(NULL));

	for (int i = 0; i < NUMBER; i++) 
		ani[NUMBER] = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3Àå ½Ç½À 4");
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutDisplayFunc(drawScene);
	glutTimerFunc(50, Timer, 1);
	glutMainLoop();
}


GLvoid drawScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < NUMBER; i++) {
		if (mx[i] != 0 && my[i] != 0) {
			glColor4f((rand() % 10) % 10 * 0.1, (rand() % 10) % 10 * 0.1, (rand() % 10) % 10 * 0.1, 0);
			glRectf(mx[i], my[i], mx[i] + size[i], my[i] + size[i]);
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
		++NUM;

		if (NUM > NUMBER - 1) {
			NUM = 0;
		}

	glutPostRedisplay();
	}
}

void Timer(int value) {
	for (int i = 0; i < NUMBER; i++) {
		size[i] += ani[i];
		if (size[i] >= 50)	ani[i] = -1;
		else if (size[i] <= 30)	ani[i] = 1;
	}
	glutPostRedisplay();
	glutTimerFunc(50, Timer, 1);
}