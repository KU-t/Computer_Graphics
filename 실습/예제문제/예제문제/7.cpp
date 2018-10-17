#include <GL/freeglut.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define PI 3.141592
#define NUMBER 10

enum { Y, N };

class Shape {
public:
	int mx, my;
	float size;
	bool exist;
	int Random;

	Shape(int _mx = 0, int _my = 0, float _size = 1, bool _exist = false) {}
};

Shape s[NUMBER];
int NUM = 0;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void drawPoint(int x, int y, int r);
void Timer(int value);


void main(int argc, char **argv) {
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3¿Â Ω«Ω¿ 7");
	glutDisplayFunc(drawScene);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutTimerFunc(100, Timer, 1);
	glutMainLoop();
}

GLvoid drawScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < NUMBER; i++) {
		
		glColor4f(rand()%100 *0.01f, rand() % 100 * 0.01f, rand() % 100 * 0.01f, 1.0f);
		
		if (s[i].exist) {
			drawPoint(s[i].mx, s[i].my, s[i].size);
		}
	}

	glFlush();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		s[NUM].exist = true;
		s[NUM].mx = x;
		s[NUM].my = 600 - y;
		s[NUM].size = rand() % 20 + 50;
		s[NUM].Random = rand() % 2;
		++NUM;

		if (NUM > NUMBER - 1) {
			NUM = 0;
		}
	}

	glutPostRedisplay();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0, 800.0, 0, 600.0, -1.0, 1.0);
}

void drawPoint(int x, int y, int r) {

	float rad = 0;

	glBegin(GL_POINTS);

	for (int i = 0; i < 360; i++) {
		rad = (float)i*PI / 180;
		glVertex2i((x + r / 2 * sin(rad)), (y + r / 2 * cos(rad)));
	}

	glEnd();
}

void Timer(int value) {
	for (int i = 0; i < NUMBER; i++) {
		if (s[i].exist == true) {
			s[i].size *= 1.1;
			if (s[i].Random == N && s[i].size > 300)
				s[i].size = rand() % 20 + 50;
			else if (s[i].size > 2000)
				s[i].exist = false;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(100, Timer, 1);
}
